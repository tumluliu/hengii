/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  logger
 *
 *        Version:  1.0
 *        Created:  05/05/2012 08:57:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  LOG_H_
#define  LOG_H_

#include <sstream>

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

const std::string LOGFILE = "run.log";
const std::string LOGLVLKEY = "LOGLVL";

/*
 * =====================================================================================
 *        Class:  Log
 *  Description:  logger
 * =====================================================================================
 */
class Log
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Log ();                             /* constructor */
		~Log();

		/* ====================  ACCESSORS     ======================================= */
		std::ostringstream &Get(enum Severity);
		std::ostringstream &Get(enum EventSource, enum Severity);
		std::ostringstream &Get(enum LogOutputType, enum Severity);
		std::ostringstream &Get(enum LogOutputType, enum EventSource, enum Severity);
		std::ostringstream &Fatal() { return Get(FATAL); }
		std::ostringstream &Error() { return Get(ERROR); }
		std::ostringstream &Warn()  { return Get(WARN); }
		std::ostringstream &Info()  { return Get(INFO); }
		std::ostringstream &Debug() { return Get(DEBUG); }

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */
		enum LogOutputType where_;
		enum Severity lvl_;
		std::ostringstream os_;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
}; /* -----  end of class Log  ----- */

#endif
