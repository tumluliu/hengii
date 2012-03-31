/*
 * =====================================================================================
 *
 *       Filename:  torquejob.cpp
 *
 *    Description:  implementation codes of invoking Torque PBS to execute HPC jobs.
 *
 *        Version:  0.6
 *        Created:  03/17/2012 11:46:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/time.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "torquejob.h"
#include "utility.h"
#include "config.h"

extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

string TorqueJob::getOutput() const {
	return output;
}

string TorqueJob::getStatus() const {
	return status;
}

int TorqueJob::getConnection() const {
	return connection;
}
		
string TorqueJob::getId() const {
	return id;
}

int TorqueJob::setProcessCount(int value) {
	if (value > 0) {
		processCount = value;
	} 
	else
		return -1;
	return 0;
}

void TorqueJob::setCmdline(const string& value) {
	cmdline = value;
}

string TorqueJob::getCmdline() const {
	return cmdline;
}

string TorqueJob::generateNameByTime() {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	stringstream year, month, day, hour, minute, second;
	year << now->tm_year + 1900;
	month << now->tm_mon + 1;
	day << now->tm_mday;
	hour << now->tm_hour;
	minute << now->tm_min;
	second << now->tm_sec;
	return year.str() + month.str() + day.str() + hour.str() + minute.str() + second.str();
}

int TorqueJob::submit() {
	string scriptFileName = generateNameByTime();
	int exist;
	while ((exist = access((PBS_OUT_DIR + scriptFileName).c_str(), F_OK)) == 0) {
		scriptFileName = generateNameByTime();
	}
	scriptPath = PBS_OUT_DIR + scriptFileName;
	outputPath = PBS_OUT_DIR + scriptFileName + PBS_OUTPUT_FILE_EXT;
	errlogPath = PBS_OUT_DIR + scriptFileName + PBS_ERRLOG_FILE_EXT;

	ofstream outputFile(scriptPath.c_str(), ios::out);
	if (!outputFile) {
		cout << "open file " << scriptPath << " error!" << endl;
		return -1;
	}
	outputFile << cmdline << endl;
	outputFile.close();

	// Here the semantic is the higine MUST be installed on 
	// the SAME server as the PBS server. But is that true 
	// in the real environment? I doubt it.
	// by Liu Lu
	// 2012/3/17
	char hostname[PARAM_SIZE];
	gethostname(hostname, sizeof(hostname));
	connection = pbs_connect(hostname); 
	if (connection < 0) {
		cout << "Connect to Torque PBS server failed!" << endl;
		cout << "ERROR " << pbs_errno << ": " << pbs_strerror(pbs_errno) << endl;
		return -1;
	}
	
	struct attropl qSubAttr[3];
	string qJobName = ATTR_N;
	qSubAttr[0].name = const_cast<char*>(qJobName.c_str());
	qSubAttr[0].value = const_cast<char*>(scriptPath.c_str());
	qSubAttr[0].resource = '\0';
	qSubAttr[0].next = &qSubAttr[1];
	string qOutputPath = ATTR_o;
	qSubAttr[1].name = const_cast<char*>(qOutputPath.c_str());
	qSubAttr[1].value = const_cast<char*>(outputPath.c_str());
	qSubAttr[1].resource = '\0';
	qSubAttr[1].next = &qSubAttr[2];
	string qErrorPath = ATTR_e;
	qSubAttr[2].name = const_cast<char*>(qErrorPath.c_str());
	qSubAttr[2].value = const_cast<char*>(errlogPath.c_str());
	qSubAttr[2].resource = '\0';
	qSubAttr[2].next = NULL;
	// !!!! The commented codes below may cause some unpredictable PBS error.
	// !!!! But the reason is not clear yet.
	// by Liu Lu
	//2012/3/17
	//qSubAttr[2].next = &qSubAttr[3];
	//string qResourceList = ATTR_l;
	//qSubAttr[3].name = const_cast<char*>(qResourceList.c_str());
	//stringstream pcs;
	//pcs << processCount;
	//qSubAttr[3].value = const_cast<char*>(pcs.str().c_str());
	//string qResource = "procs";
	//qSubAttr[3].resource = const_cast<char*>(qResource.c_str());
	//qSubAttr[3].next = NULL;

	char *ret = pbs_submit(connection, qSubAttr, const_cast<char*>(scriptPath.c_str()), 0, 0);
	if (!ret)
	{
		cout << endl << "================================================" << endl;
		cout << "Torque PBS job submission failed!" << endl;
		cout << "ERROR " << pbs_errno << ": " << pbs_strerror(pbs_errno) << endl;
		return -1;
	}
	id = ret;
	cout << "Torque PBS job has been submitted successfully, job ID is: " << id << endl;

	return 0;
}

int TorqueJob::collect() {
	struct attrl qStatAttr;
	string qJobState = ATTR_state;
	qStatAttr.name = const_cast<char*>(qJobState.c_str());
	char value[] = "\0";
	qStatAttr.value = value;
	char resource[] = "\0";
	qStatAttr.resource = resource;
	qStatAttr.next = NULL;

	struct batch_status *qStatus;
	do {
		do {
			qStatus = pbs_statjob(connection, const_cast<char*>(id.c_str()), &qStatAttr, 0);
			// sleep(1);
		} while (qStatus == NULL);
		status = (qStatus->attribs)->value;	 
		cout << "The status is:" << status << endl;
		// !!!! I have a feeling that there must be some problems here !!!!
		// !!!! commented by Liu Lu !!!!
	} while (status != "C");

	if (Utility::readFile(outputPath, output) != 0) {
		return -1;
	}
	if ((Utility::deleteFile(scriptPath)) != 0) {
		return -1;
	}
	if ((Utility::deleteFile(outputPath)) != 0) {
		return -1;
	}
	pbs_statfree(qStatus);
	pbs_disconnect(connection);

	return 0;
}


