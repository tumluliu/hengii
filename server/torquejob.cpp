/*
 * =====================================================================================
 *
 *       Filename:  torquejob.cpp
 *
 *    Description:  implementation codes of invoking Torque PBS to execute HPC jobs.
 *
 *        Version:  0.7
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
#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "torquejob.h"
#include "utility.h"
#include "logger.h"
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
	outputPath = scriptPath + PBS_OUTPUT_FILE_EXT;
	errlogPath = scriptPath + PBS_ERRLOG_FILE_EXT;

	ofstream outputFile(scriptPath.c_str(), ios::out);
	if (!outputFile) {
		Logger::log(STDOUT, ERROR, TORQUE, "unable to open torque script file " + scriptPath);
		return -1;
	}
	outputFile << cmdline << endl;
	outputFile.close();

	connection = pbs_connect(const_cast<char*>(PBS_SERVER_HOST.c_str())); 
	if (connection < 0) {
		Logger::log(STDOUT, ERROR, TORQUE, "Connect to Torque PBS server failed!");
		Logger::log(STDOUT, ERROR, TORQUE, pbs_strerror(pbs_errno));
		return -1;
	}

	char hostPrefix[PARAM_SIZE];
	gethostname(hostPrefix, PARAM_SIZE);
	strncat(hostPrefix, ":", 1);
	const string prefix(hostPrefix);
	string fullloc;

	struct attropl qSubAttr[3];
	// ATTR_N is defined as "Job_Name"
	qSubAttr[0].name = (char *)malloc(strlen(ATTR_N) + 1);
	if (qSubAttr[0].name == NULL) {
		Logger::log(STDOUT, FATAL, ENGINE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[0].name, ATTR_N, strlen(ATTR_N) + 1);
	qSubAttr[0].value = (char *)malloc(scriptPath.size() + 1);
	if (qSubAttr[0].value == NULL) {
		Logger::log(STDOUT, FATAL, ENGINE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[0].value, scriptPath.c_str(), scriptPath.size() + 1);
	qSubAttr[0].resource = (char *)NULL;
	qSubAttr[0].next = &qSubAttr[1];
	// ATTR_o is defined as "Output_Path"
	qSubAttr[1].name = (char *)malloc(strlen(ATTR_o) + 1);
	if (qSubAttr[1].name == NULL) {
		Logger::log(STDOUT, FATAL, ENGINE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[1].name, ATTR_o, strlen(ATTR_o) + 1);
	fullloc = prefix + outputPath;
	qSubAttr[1].value = (char *)malloc(fullloc.size() + 1);
	if (qSubAttr[1].value == NULL) {
		Logger::log(STDOUT, FATAL, TORQUE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[1].value, fullloc.c_str(), fullloc.size() + 1);
	Logger::log(STDOUT, DEBUG, TORQUE, "output location: " + fullloc);
	qSubAttr[1].resource = (char *)NULL;
	qSubAttr[1].next = &qSubAttr[2];
	// ATTR_e is defined as "Error_Path"
	qSubAttr[2].name = (char *)malloc(strlen(ATTR_e) + 1);
	if (qSubAttr[2].name == NULL) {
		Logger::log(STDOUT, FATAL, ENGINE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[2].name, ATTR_e, strlen(ATTR_e) + 1);
	fullloc = prefix + errlogPath;
	qSubAttr[2].value = (char *)malloc(fullloc.size() + 1);
	if (qSubAttr[2].value == NULL) {
		Logger::log(STDOUT, FATAL, TORQUE, "out of memory");
		return -1;
	}
	strncpy(qSubAttr[2].value, fullloc.c_str(), fullloc.size() + 1);
	Logger::log(STDOUT, DEBUG, TORQUE, "error location: " + fullloc);
	qSubAttr[2].resource = (char *)NULL;
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
		Logger::log(STDOUT, ERROR, TORQUE, "Torque PBS job submission failed!");
		Logger::log(STDOUT, ERROR, TORQUE, pbs_strerror(pbs_errno));
		return -1;
	}
	id = string(ret);
	stringstream msg;
	msg << "Torque PBS job has been submitted successfully, job ID is: " << id << endl;
	Logger::log(STDOUT, INFO, TORQUE, msg.str());

	for (int i = 0; i < 3; i++) {
		free(qSubAttr[i].name);
		free(qSubAttr[i].value);
	}

	return 0;
}

int TorqueJob::collect() {
	if (Utility::readFile(outputPath, output) != 0) {
		return -1;
	}
	if ((Utility::deleteFile(scriptPath)) != 0) {
		return -1;
	}
	if ((Utility::deleteFile(outputPath)) != 0) {
		return -1;
	}

	return 0;
}
