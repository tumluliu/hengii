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
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "logger.h"
#include "config.h"

void Logger::log(LogOutputType output, Severity level, EventSource sender, const string& msg) {
	string severityLvl;
	switch (level) {
		case FATAL:
			severityLvl = "FATAL";
			break;
		case ERROR:
			severityLvl = "ERROR";
			break;
		case WARN:
			severityLvl = "WARN";
			break;
		case INFO:
			severityLvl = "INFO";
			break;
		case DEBUG:
			severityLvl = "DEBUG";
			break;
		default:
			break;
	}
	string eventSender; 
	switch (sender) {
		case APPLICATION:
			eventSender = "USER APP";
			break;
		case TORQUE:
			eventSender = "TORQUE PBS";
			break;
		case MPI_ENV:
			eventSender = "MPI";
			break;
		case DATABASE:
			eventSender = "DATABASE";
			break;
		default:
			break;
	}
	time_t t = time(0);
	string now = string(asctime(localtime(&t)));
	string logInfo = "--" + now + severityLvl + ", " + eventSender + ", " + msg; 

	switch ( output ) {
		case STDOUT:	
			cout << logInfo << endl;
			break;

		case STDERR:	
			cerr << logInfo << endl;
			break;

		case LOG_FILE:
			{ 	
				ofstream logFile;
				logFile.open(RUN_LOG_FILE.c_str(), ofstream::app);
				if (!logFile) {
					cerr << "error: unable to open higine log file: "
						<< RUN_LOG_FILE << endl;
					return;
				}
				logFile << logInfo << endl;
				logFile.close();
				break;
			}

		case LOG_DB:
			//write log to database
			cout << "Write to database: " << logInfo << endl;
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
}
