/*
 * =====================================================================================
 *
 *       Filename:  joblog.cpp
 *
 *    Description:
 *
 *        Version:  0.7
 *        Created:  04/08/2012 03:11:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani()
 *         Updator: LIU Lu() YANG Anran()
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string>
#include "joblog.h"
#include "utility.h"
#include "logger.h"

JobLog *JobLog::m_instance;

JobLog *JobLog::Instance() {
	if (!m_instance) {
		// !!! it is some(although not very bad) memory leak here!!! by YANG Anran at 20120413
		m_instance = new JobLog();
		m_instance->initDb();
	}
	return m_instance;
}

JobLog::JobLog(): hosts(DB_SERVER),userName(DB_USER),password(DB_PASSWORD),dbName(DB_NAME),port(DB_PORT){ }

JobLog::~JobLog() {
	//cout << conn << endl;
	//if ( conn ) {
	//cout << "close connection" << endl;
	////mysql_close(conn);
	//}
}

int JobLog::initDb() {
	conn = mysql_init( NULL );
	if ( !conn ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql init failed." );
	}

	my_bool reconnect = 1;
	mysql_options( conn, MYSQL_OPT_RECONNECT, &reconnect );

	conn = mysql_real_connect( conn, hosts.c_str(), userName.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0 );
	if ( conn ) {
		Logger::log( STDOUT, INFO, DATABASE, "mysql connection successful." );
		return 0;
	}
	else {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql connection failed." );
		return -1;
	}
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in initDb" );
		return -1;
	}
}

string JobLog::registerJobSql( int64_t flowId, int jobId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in registerJobSql" );
	}
	return "insert into " + JOB_TABLE_NAME
		+ "(fid, id) values('" + Utility::intToString( flowId )
		+ "', '" + Utility::intToString( jobId ) + "');";
}

string JobLog::registerPbsJobSql( int64_t flowId, int jobId, const string &pbsJobId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in registerPbsJobSql" );
	}
	return "insert into " + PBS_JOB_TABLE_NAME
		+ "(fid, jobid, id) values('" + Utility::intToString( flowId )
		+ "', '" + Utility::intToString( jobId ) + "', '" + pbsJobId + "');";
}

int JobLog::registerJob( int64_t flowId, int jobId, const string &pbsJobId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in registerJob" );
		return -1;
	}
	if ( command( registerJobSql( flowId, jobId ) ) == 0 ) {
		return command( registerPbsJobSql( flowId, jobId, pbsJobId ) );
	}
	else {
		return -1;
	}
}

string JobLog::registerJobFlowSql( int64_t flowId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in registerJobFlowSql" );
	}
	return "insert into " + JOB_FLOW_TABLE_NAME + "(id) values('" + Utility::intToString( flowId ) + "');";
}

int JobLog::registerJobFlow( int64_t flowId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in registerJobFlow" );
		return -1;
	}
	return command( registerJobFlowSql( flowId ) );
}

string JobLog::updateJobStatusSql( int64_t flowId, int jobId, int status, const string &output ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in updateJobStatusSql" );
	}
	return "update " + JOB_TABLE_NAME + " set status=" + Utility::intToString( status ) + ", message='" + output
		+ "' where fid='" + Utility::intToString( flowId ) + "' and id='" + Utility::intToString( jobId ) + "';";
}

int JobLog::updateJobStatus( int64_t flowId, int jobId, int status, const string &output ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in updateJobStatus" );
		return -1;
	}
	return command( updateJobStatusSql( flowId, jobId, status, output ) );
}

string JobLog::getPbsJobStatusSql( int64_t flowId, int jobId ) {
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in getPbsJobStatusSql" );
	}
	return "select job_state from " + PBS_JOB_TABLE_NAME
		+ " where fid='" + Utility::intToString( flowId )
		+ "' and jobid='" + Utility::intToString( jobId ) + "';";
}

char JobLog::getPbsJobStatus( int64_t flowId, int jobId ) {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;
	char status;

	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in getPbsJobStatus" );
	}

	res = query( getPbsJobStatusSql( flowId, jobId ) );
	if ( res != NULL ) {
		r = mysql_fetch_row( res );
	}

	if ( r == NULL || r[0] == NULL ) {
		return 'N';
	}

	status = r[0][0];
	mysql_free_result( res );

	return status;
}

int JobLog::command( const string &command ) {
	int ret;
	int ping;
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in command" );
		return -1;
	}

	if ( !conn ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql connection lost" );
		return -1;
	}

	ret = mysql_query( conn, command.c_str() );
	if ( ret != 0 ) {
		string errMsg( mysql_error( conn ) );
		Logger::log( STDOUT, ERROR, DATABASE, "mysql command error: " + errMsg );
	}
	else {
		Logger::log( STDOUT, INFO, DATABASE, "mysql command executed successfully." );
	}

	return 0;
}

MYSQL_RES *JobLog::query( const string &query ) {
	int ret;
	MYSQL_RES *res;
	int ping;

	//if ( !conn ) {
	//Logger::log( STDOUT, ERROR, DATABASE, "mysql connection lost" );
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error" );
		return NULL;
	}
	//return NULL;
	//}
	ret = mysql_query( conn, query.c_str() );
	res = mysql_store_result( conn );
	if ( ret != 0 ) {
		string errMsg( mysql_error( conn ) );
		Logger::log( STDOUT, ERROR, DATABASE, "mysql query error: " + errMsg );
	}
	else
		Logger::log(STDOUT, DEBUG, DATABASE, "mysql query executed successfully.");

	return res;
}
