/*
 * =====================================================================================
 *
 *       Filename:  logger.cpp
 *
 *    Description:  implementation of class Logger
 *
 *        Version:  0.6
 *        Created:  04/05/2012 07:46:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <logger.h>
#include <fstream>
#include <unistd.h>

void Logger::log(LogOutputType output, Severity level, EventSource sender, const string& msg)
{
	string severityLvl;
	switch (level) {
		case Severity.FATAL:
			severityLvl = "fatal";
			break;
		case Severity.ERROR:
			severityLvl = "error";
			break;
		case Severity.WARN:
			severityLvl = "warn";
			break;
		case Severity.INFO:
			severityLvl = "info";
			break;
		case Severity.DEBUG:
			severityLvl = "debug";
			break;
		default:
			break;
	}
	string eventSender; 
	switch (sender) {
		case EventSource.APPLICATION:
			eventSender = "user app";
			break;
		case EventSource.TORQUE:
			eventSender = "torque pbs";
			break;
		case EventSource.MPI_ENV:
			eventSender = "mpi";
			break;
		case EventSource.DATABASE:
			eventSender = "database";
			break;
		default:
			break;
	}
	time_t t = time(0);
	string now = asctime(localtime(&t)).str();
	string logInfo = now + ", " + severityLvl + ", " + eventSender + ", " + msg; 
	string logFilePath = LOG_DIR + LOG_FILE_NAME;
	ofstream logFile;
	logFile.open(logFilePath.c_str(), ofstream::app);
	if (!logFile) {
		cerr << "error: unable to open higine log file: "
			<< logFilePath << endl;
		return;
	}

	time_t t;
	fp = fopen(file.c_str(), "a");
	if ((fp) >= 0) {
		t = time(0);
		fprintf(fp, "%s:%s\n", asctime(localtime(&t)), content.c_str());
		fclose(fp);
	}
	logFile.close();
}
