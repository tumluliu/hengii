/*
 * =====================================================================================
 *
 *       Filename:  log.cpp
 *
 *    Description:  logger
 *
 *        Version:  1.0
 *        Created:  05/05/2012 09:03:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdio.h>

#include "utility.h"
#include "log.h"

std::string severityToString(enum Severity lvl) {
	std::string lvlstr;

	switch (lvl) {
		case FATAL:
			lvlstr = "FATAL";
			break;
		case ERROR:
			lvlstr = "ERROR";
			break;
		case WARN:
			lvlstr = "WARN";
			break;
		case INFO:
			lvlstr = "INFO";
			break;
		case DEBUG:
			lvlstr = "DEBUG";
			break;
		default:
			break;
	}

	return lvlstr;
}

std::string eventSourceToString(enum EventSource sender) {
	std::string senderstr;

	switch (sender) {
		case APPLICATION:
			senderstr = "USER APP";
			break;
		case TORQUE:
			senderstr = "TORQUE PBS";
			break;
		case MPI:
			senderstr = "MPI";
			break;
		case DATABASE:
			senderstr = "DATABASE";
			break;
		case ENGINE:
			senderstr = "ENGINE";
			break;
		default:
			break;
	}

	return senderstr;
}

Log::Log() : where_(STDOUT), lvl_(INFO), os_() {}

std::ostringstream &Log::Get(enum LogOutputType t, enum EventSource source, enum Severity lvl) {
	where_ = t;
	lvl_ = lvl;
	os_ << "- " << util::getCurrentTime();
	os_ << " " << eventSourceToString(source);
	os_ << " " << severityToString(lvl) << ": ";
	return os_;
}

std::ostringstream &Log::Get(enum LogOutputType t, enum Severity lvl) {
	return Get(t, ENGINE, lvl);
}

std::ostringstream &Log::Get(enum EventSource source, enum Severity lvl) {
	return Get(STDOUT, source, lvl);
}

std::ostringstream &Log::Get(enum Severity lvl) {
	return Get(STDOUT, ENGINE, lvl);
}

Log::~Log() {	
	os_ << std::endl;

	FILE *loc;
	switch(where_) {
		case STDOUT:
			loc = stdout;
			break;
		case STDERR:
			loc = stderr;
			break;
		case LOG_FILE:
			loc = fopen(LOGFILE.c_str(), "a");
			break;
		default:
			loc = stdout;
			break;
	}

	char* lvlenv = getenv(LOGLVLKEY.c_str());
	int displaylvl = 1;
	if (lvlenv != NULL) {
		displaylvl = atoi(lvlenv);
	}

	if (displaylvl >= lvl_) {
		fprintf(loc, "%s", os_.str().c_str());
		fflush(loc);
	}

	if (where_ == LOG_FILE) {
		fclose(loc);
	}
}
