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

/* Here the global unique id is set. Although one jobtracker only has one torque job so 
 * there is unambigious when torque job notice the jobtracker, torque job itsself must 
 * access some global resources, like pbs server and file system. So it's unacceptable if 
 * two torque job are bitwisely the same. */
TorqueJob::TorqueJob(int64_t id, const Resources &resources, const string &cmdline) 
	: Player(id), pbsid_(""), resources_(resources), cmdline_(cmdline), connection_(-1),
	scriptpath_(""), outputpath_(""), errlogpath_(""),
	output_(""), sig_(BatchSignal::CONTINUE) { 
	}

string TorqueJob::get_output() const {
	return output_;
}

void TorqueJob::Play() {
	Log().Debug() << "recorder count is: " << get_recorders_num();
	ExitReason::type baton = ExitReason::NORMAL;

	baton = CreateScript();
	if (baton == ExitReason::FAILED) {
		SendFailed();
		return;
	}

	baton = Connect();
	if (baton == ExitReason::FAILED) {
		util::deleteFile(scriptpath_);
		SendFailed();
		return;
	}

	baton = Submit();
	if (baton == ExitReason::FAILED) {
		util::deleteFile(scriptpath_);
		pbs_disconnect(connection_);
		SendFailed();
		return;
	}
	util::deleteFile(scriptpath_);

	baton = Trace();
	if (baton == ExitReason::FAILED) {
		pbs_disconnect(connection_);
		SendFailed();
		return;
	}
	else if (baton == ExitReason::CANCELED) {
		Collect();
		pbs_disconnect(connection_);
		SendCanceled();
		return;
	}

	baton = Collect();
	if (baton == ExitReason::FAILED) {
		pbs_disconnect(connection_);
		SendFailed();
		return;
	}

	pbs_disconnect(connection_);
	SendFinished();
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
	return util::intToString(get_id());
}

ExitReason::type TorqueJob::CreateScript() {
	scriptpath_ = PBS_OUT_DIR + GenerateScriptName();

	std::ofstream outputFile(scriptpath_.c_str(), std::ios::out);
	if (outputFile.fail()) {
		Log().Error() << "unable to open torque script file " + scriptpath_;
		return ExitReason::FAILED;
	}

	outputFile << cmdline_ << std::endl;
	outputFile.close();
	return ExitReason::NORMAL;
}

ExitReason::type TorqueJob::Connect() {
	connection_ = pbs_connect(const_cast<char*>(PBS_SERVER_HOST.c_str())); 
	if (connection_ < 0) {
		Log().Get(TORQUE, ERROR) 
			<< "Connect to Torque PBS server failed! Error number is "
			<<  pbs_strerror(pbs_errno);
		return ExitReason::FAILED;
	}
	return ExitReason::NORMAL;
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

const string TorqueJob::ReqResourceStr() const {
	std::stringstream ss;
	ss << resources_.get_nodes() << ":ppn=" << resources_.get_ppn();
	return ss.str();
}

void TorqueJob::FillAttr(PbsAttr &attr) {
	outputpath_ = scriptpath_ + PBS_OUTPUT_FILE_EXT;
	errlogpath_ = scriptpath_ + PBS_ERRLOG_FILE_EXT;

	attr.Add(ATTR_o, LocWithHost(outputpath_));
	attr.Add(ATTR_e, LocWithHost(errlogpath_));
	attr.AddResource("nodes", ReqResourceStr());

	Log().Debug() << "pbs job " << pbsid_ << "output in: " << outputpath_;
	Log().Debug() << "pbs job " << pbsid_ << "errlog in: " << errlogpath_;
}

ExitReason::type TorqueJob::Submit() {
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
		return ExitReason::FAILED;
	}
	pbsid_ = string(ret);
	free(ret);

	Log().Get(TORQUE, INFO) 
		<< "Torque PBS job has been submitted successfully, job ID is: "
		<< pbsid_
		<< ", cmdline is: "
		<< cmdline_;

	return ExitReason::NORMAL;
}

char TorqueJob::GetPbsStatus() const {
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

ExitReason::type TorqueJob::Trace() {
	char status = 'Q';
	while(status != 'C') { 
		status = GetPbsStatus();

		if (sig_ == BatchSignal::CANCEL) {
			CancelPbsJob(); 
			return ExitReason::CANCELED;
		}
		if (status == 0) {
			return ExitReason::FAILED;
		}

		usleep(UPDATE_INTERVAL_MS);
	}
	return ExitReason::NORMAL;
}

ExitReason::type TorqueJob::Collect() {
	string stderr_output;
	if (util::readFile(outputpath_, output_) != 0) {
		/* WARNING: Here should be more carefully thought. by YANG Anran 
		 * @ 2012.5.25 */
		/* wait a minute and retry */
		sleep(WAIT_FOR_OUT_S);
		if (util::readFile(outputpath_, output_) != 0) {
			return ExitReason::FAILED;
		}
	}
	if (util::readFile(errlogpath_, stderr_output) != 0) {
		return ExitReason::FAILED;
	}

	std::stringstream ss;
	ss << output_;
	if (stderr_output != "") {
		ss << std::endl << "============ Error msg ===============" << std::endl;
		ss << stderr_output; 
	}
	output_ = ss.str();

	util::deleteFile(outputpath_);
	util::deleteFile(errlogpath_);

	Log().Debug() << "torque output is: " << output_;

	return ExitReason::NORMAL;
}

void TorqueJob::Stop() {
	sig_ = BatchSignal::CANCEL;
}

void TorqueJob::CancelPbsJob() {
	pbs_deljob(connection_, const_cast<char*>(pbsid_.c_str()), NULL);
}

void TorqueJob::SendFinished() {
	if(auto boss = get_bossrecorder().lock()) { boss->OnePlayerDone(get_id()); };
}

void TorqueJob::SendFailed() {
	/* WARNING: not yet implemented by YANG Anran @ 2012.5.25 */
	//if(auto boss = get_bossrecorder().lock()) { boss->OnePlayerFailed(get_id()); };
}

void TorqueJob::SendCanceled() {
	if(auto boss = get_bossrecorder().lock()) { boss->OnePlayerCanceled(get_id()); };
}
