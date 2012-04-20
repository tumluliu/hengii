/*
 * =====================================================================================
 *
 *       Filename:  logger.h
 *
 *    Description:  Declaration of class Logger
 *
 *        Version:  0.9
 *        Created:  04/05/2012 04:07:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _LOGGER_H_ 
#define _LOGGER_H_ 

#include <string>
#include <config.h>

using namespace std;

enum EventSource {
	APPLICATION = 1,
	TORQUE = 2,
	MPI = 3,
	DATABASE = 4,
	ENGINE = 5
};

enum LogOutputType {
	STDOUT = 1,
	STDERR = 2,
	LOG_FILE = 3,
	LOG_DB = 4
};

enum Severity {
	FATAL = 1,
	ERROR = 2,
	WARN = 3,
	INFO = 4,
	DEBUG = 5
};

class Logger {
	public:
		static void log(LogOutputType, Severity, EventSource, const string&);
};

#endif
