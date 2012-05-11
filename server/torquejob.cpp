/*
 * =====================================================================================
 *
 *       Filename:  torquejob.cpp
 *
 *    Description:  the declaration of job class submitted to Torque PBS.
 *
 *        Version:  1.0
 *        Created:  03/17/2012 03:38:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *                  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <sys/time.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <unistd.h>

#include "torquejob.h"
#include "utility.h"
#include "log.h"
#include "config.h"
#include "resources.h"

extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

using std::string;

TorqueJob::TorqueJob(int processcount, const string &cmdline) 
	: Player(-1), pbsid_(""), processcount_(processcount), cmdline_(cmdline), connection_(-1),
	scriptpath_(""), outputpath_(""), errlogpath_(""),
	output_("") { 
	}

string TorqueJob::get_output() const {
	return output_;
}

string TorqueJob::GenerateNameByTime() const {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::stringstream year, month, day, hour, minute, second;
	year << now->tm_year + 1900;
	month << now->tm_mon + 1;
	day << now->tm_mday;
	hour << now->tm_hour;
	minute << now->tm_min;
	second << now->tm_sec;
	return year.str() + month.str() + day.str() + hour.str() + minute.str() + second.str();
}

string TorqueJob::GenerateScriptName() const {
	string name;
	int exist;
	do {
		name = GenerateNameByTime();
	} while ((exist = access((PBS_OUT_DIR + name).c_str(), F_OK)) == 0);
	return name;
}

int TorqueJob::CreateScript() {
	scriptpath_ = PBS_OUT_DIR + GenerateScriptName();

	std::ofstream outputFile(scriptpath_.c_str(), std::ios::out);
	if (outputFile.fail()) {
		Log().Error() << "unable to open torque script file " + scriptpath_;
		/* need error handling */
		return -1;
	}

	outputFile << cmdline_ << std::endl;
	outputFile.close();
	return 0;
}

int TorqueJob::Connect() {
	connection_ = pbs_connect(const_cast<char*>(PBS_SERVER_HOST.c_str())); 
	if (connection_ < 0) {
		Log().Get(TORQUE, ERROR) 
			<< "Connect to Torque PBS server failed! Error number is "
			<<  pbs_strerror(pbs_errno);
		return -1;
	}
	return 0;
}

const string TorqueJob::LocWithHost(const string &partial) const {
	const int HOSTNAME_SIZE = 1024;
	char prefix[HOSTNAME_SIZE + 1]; /* contains a ':' */

	/* fill in host name */
	prefix[HOSTNAME_SIZE - 1] = '\0';
	gethostname(prefix, HOSTNAME_SIZE - 1);

	/* add ':' */
	strncat(prefix, ":", 1);
	return string(prefix) + partial;
}

const string TorqueJob::ReqResourceStr(int processes) const {
	Resources rsrc;
	rsrc.Request(processes);

	std::stringstream ss;
	ss << rsrc.get_nodes() << ":ppn=" << rsrc.get_ppn();
	return ss.str();
}

void TorqueJob::FillAttr(PbsAttr &attr) {
	outputpath_ = scriptpath_ + PBS_OUTPUT_FILE_EXT;
	errlogpath_ = scriptpath_ + PBS_ERRLOG_FILE_EXT;

	attr.Add(ATTR_o, LocWithHost(outputpath_));
	attr.Add(ATTR_e, LocWithHost(errlogpath_));
	attr.AddResource("nodes", ReqResourceStr(processcount_));

	Log().Debug() << "pbs job " << pbsid_ << "output in: " << outputpath_;
	Log().Debug() << "pbs job " << pbsid_ << "errlog in: " << errlogpath_;
}

int TorqueJob::Submit() {
	PbsAttr attr;
	FillAttr(attr);

	Log().Get(TORQUE, DEBUG) << "Submitting job, outpath is "
		<< scriptpath_.c_str();
	char *ret = pbs_submit(connection_, attr.MakeCAttrl(), 
			const_cast<char*>(scriptpath_.c_str()), 0, 0);

	if (!ret)
	{
		Log().Get(TORQUE, ERROR) 
			<< "Torque PBS job submission failed! Error no. is: "
			<< pbs_strerror(pbs_errno);
		return -1;
	}
	pbsid_ = string(ret);
	free(ret);

	Log().Get(TORQUE, INFO) 
		<< "Torque PBS job has been submitted successfully, job ID is: "
		<< pbsid_
		<< ", cmdline is: "
		<< cmdline_;

	return 0;
}

void TorqueJob::Fail() {
	/* do some error handling here */
}

void TorqueJob::Play() {
	/* A trick that using short-circuit.
	 * The following actions will be taken one by one,
	 * and if one fails(return value is not 0), the flow
	 * will terminate immediately and do some error handling work */
	if (
			CreateScript() != 0 ||
			Connect() != 0 ||
			Submit() != 0 ||
			Trace() != 0 ||
			Collect() != 0) {
		Fail();
	}
	Exit();
}

char TorqueJob::GetStatus() const {
	char result = 0;

	/* WARNING: These rather ugly code is due to the strange distinction
	 * between attropl and attrl. In future a conversion method
	 * may be implemented if needed. by YANG Anran @ 2012.5.8 */
	PbsAttr attr;
	attr.Add(ATTR_state, "");
	struct attropl *popl = attr.MakeCAttrl();
	struct attrl l; 
	l.name = popl->name;
	l.resource = NULL;
	l.value = NULL;
	l.next = NULL;

	char *cid = (char *)malloc(pbsid_.size() + 1);
	memset(cid, '\0', pbsid_.size() + 1);
	strncpy(cid, pbsid_.c_str(), pbsid_.size());
	struct batch_status *stat = pbs_statjob(connection_, cid, &l, 0);
	free(cid);

	if (stat == NULL) {
		Log().Get(TORQUE, ERROR) << "Unable to query pbs status";
		return 0;
	}

	/* WARNING: These some ugly code can be digest by PbsAttr, using 
	 * a constructor from attrl. But currently there's 
	 * no need to bother. by YANG Anran @ 2012.5.8 */
	struct attrl *iter = stat->attribs;
	do {
		if(strcmp(iter->name, ATTR_state) == 0) {
			result = iter->value[0];
		}
	} while(iter->next != NULL);

	pbs_statfree(stat);

	return result;
}

int TorqueJob::Trace() {
	char status = 'Q';
	while(status != 'C') { 
		status = GetStatus();

		if (status == 0) {
			return -1;
		}

		usleep(UPDATE_INTERVAL_MS);
	}
	return 0;
}

int TorqueJob::Collect() {
	if (util::readFile(outputpath_, output_) != 0) {
		return -1;
	}

	util::deleteFile(scriptpath_);
	util::deleteFile(outputpath_);

	Log().Debug() << "torque output is: " << output_;

	return 0;
}

void TorqueJob::Exit() {
	if (connection_ < 0) {
		return;
	}
	pbs_disconnect(connection_);

	EACH_RECORDER(OnePlayerDone(get_id()));
}
