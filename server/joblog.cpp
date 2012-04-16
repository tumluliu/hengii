/*
 * =====================================================================================
 *
 *       Filename:  joblog.cpp
 *
 *    Description:
 *
 *        Version:  0.8
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
#include <stdlib.h>
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
	}
	return m_instance;
}

JobLog::JobLog(): hosts(DB_SERVER),userName(DB_USER),password(DB_PASSWORD),dbName(DB_NAME),port(DB_PORT){ 
	int i;
	MYSQL *conn;

	for (i = 0; i < MAX_DB_CONN_NUM; i++) {
		if ((conn = createConn())) {
			pthread_mutex_t* lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(lock, NULL);
			connLock.push_back(lock);

			connPool.push_back(conn);
		}
		else {
			break;
		}
	}
}

MYSQL *JobLog::borrowConnection() {
	unsigned int i;

	do {
		for (i = 0; i < connPool.size(); i++) {
			//cout << "trying locking " << i << "th conn" << endl; // checking
			if(pthread_mutex_trylock(connLock[i]) == 0) {
				//cout << i << "th conn locked" << endl;
				return connPool[i];
			}
		}
	} while(true);
}

int JobLog::returnConnection(MYSQL *conn) {
	unsigned int i;
	bool known = false;

	for (i = 0; i < connPool.size(); i++) {
		if (conn == connPool[i]) {
			//cout << "unlocking " << i << "th conn" << endl;
			pthread_mutex_unlock(connLock[i]);
			//cout << i << "th conn unlocked" << endl;
			known = true;
		}
	}

	if (known) {
		return 0;
	}
	else {
		return -1;
	}
}

JobLog::~JobLog() {
	//cout << conn << endl;
	//if ( conn ) {
	//cout << "close connection" << endl;
	////mysql_close(conn);
	//}
}

MYSQL *JobLog::createConn() {
	MYSQL *conn = mysql_init( NULL );
	if ( !conn ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql init failed." );
	}

	my_bool reconnect = 1;
	mysql_options( conn, MYSQL_OPT_RECONNECT, &reconnect );

	conn = mysql_real_connect( conn, hosts.c_str(), userName.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0 );
	if ( conn ) {
		//Logger::log( STDOUT, INFO, DATABASE, "mysql connection successful." );
	}
	else {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql connection failed." );
		return NULL;
	}

	return conn;
}

string JobLog::registerJobSql( int64_t flowId, int jobId ) {
	return "insert into " + JOB_TABLE_NAME
		+ "(fid, id) values('" + Utility::intToString( flowId )
		+ "', '" + Utility::intToString( jobId ) + "');";
}

string JobLog::registerPbsJobSql( int64_t flowId, int jobId, const string &pbsJobId ) {
	return "insert into " + PBS_JOB_TABLE_NAME
		+ "(fid, jobid, id) values('" + Utility::intToString( flowId )
		+ "', '" + Utility::intToString( jobId ) + "', '" + pbsJobId + "');";
}

int JobLog::registerPbsJob( int64_t flowId, int jobId, const string &pbsJobId ) {
	return command( registerPbsJobSql( flowId, jobId, pbsJobId ) );
}

int JobLog::registerJob(int64_t flowId, int jobId) {
	return command( registerJobSql( flowId, jobId ) );
}

string JobLog::registerJobFlowSql( int64_t flowId, const string &userId ) {
	return "insert into " + JOB_FLOW_TABLE_NAME 
		+ "(id, user, ctime, utime) values('" 
		+ Utility::intToString( flowId ) + "', '" 
		+ userId + "', '" 
		+ getCurrentTime() + "', '" 
		+ getCurrentTime() + "');";
}

int JobLog::registerJobFlow( int64_t flowId, const string &userId ) {
	return command( registerJobFlowSql( flowId, userId ) );
}

string JobLog::updateJobStatusSql( int64_t flowId, int jobId, int status, const string &message ) {
	return "update " + JOB_TABLE_NAME + " set status=" + Utility::intToString( status ) 
		+ ", message='" + message
		+ "' where fid='" + Utility::intToString( flowId ) 
		+ "' and id='" + Utility::intToString( jobId ) + "';";
}

int JobLog::updateJobStatus( int64_t flowId, int jobId, int status, const string &output ) {
	return command( updateJobStatusSql( flowId, jobId, status, output ) );
}


string JobLog::updateJobFlowStatusSql(int64_t flowId, int status, const string &message) {
	return "update " + JOB_FLOW_TABLE_NAME 
		+ " set status=" + Utility::intToString( status ) 
		+ ", message='" + message 
		+ "', utime='" + getCurrentTime()
		+ "' where id='" + Utility::intToString(flowId) + "'";
}

int JobLog::updateJobFlowStatus(int64_t flowId, int status, const string &message) {
	return command( updateJobFlowStatusSql(flowId, status, message) );
}

string JobLog::getJobStatusSql( int64_t flowId, int jobId ) {
	return "select status, message from " + JOB_TABLE_NAME
		+ " where fid='" + Utility::intToString( flowId )
		+ "' and id='" + Utility::intToString( jobId ) + "';";
}

string JobLog::getJobCountSql( int64_t flowId ) {
	return "select count(*) as job_count from " + JOB_TABLE_NAME
		+ " where fid='" + Utility::intToString( flowId )
		+ "';";
}

string JobLog::getFlowStatusSql( int64_t flowId ) {
	return "select status, message from " + JOB_FLOW_TABLE_NAME
		+ " where id='" + Utility::intToString( flowId ) + "';";
}

string JobLog::getPbsJobStatusSql( int64_t flowId, int jobId ) {
	return "select job_state from " + PBS_JOB_TABLE_NAME
		+ " where fid='" + Utility::intToString( flowId )
		+ "' and jobid='" + Utility::intToString( jobId ) + "';";
}

char JobLog::getPbsJobStatus( int64_t flowId, int jobId ) {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;
	char status;

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

int JobLog::getJobStatus( int64_t flowId, int jobId, string &message ) {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;
	int status;

	res = query( getJobStatusSql( flowId, jobId ) );
	if ( res != NULL ) {
		r = mysql_fetch_row( res );
	}

	if ( r == NULL  || r[0] == NULL ) {
		return -1;
	}

	status = atoi(r[0]);

	message = r[1] ? string(r[1]) : "";

	mysql_free_result( res );

	return status;
}

int JobLog::getJobCount( int64_t flowId ) {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;
	int jobCount = 0;

	res = query( getJobCountSql( flowId ) );
	if ( res != NULL ) {
		r = mysql_fetch_row( res );
	}

	if ( r == NULL || r[0] == NULL ) {
		return -1;
	}

	jobCount = atoi(r[0]);

	mysql_free_result( res );

	return jobCount;
}

int JobLog::getFlowStatus( int64_t flowId, string &message ) {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;
	int status;

	res = query( getFlowStatusSql( flowId ) );
	if ( res != NULL ) {
		r = mysql_fetch_row( res );
	}

	if ( r == NULL || r[0] == NULL ) {
		return -1;
	}

	status = atoi(r[0]);

	message = r[1] ? string(r[1]) : "";

	mysql_free_result( res );

	return status;
}

int JobLog::command( const string &command ) {
	int ret;
	int ping;
	MYSQL *conn;

	conn = borrowConnection();
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
		Logger::log( STDOUT, ERROR, DATABASE, "mysql command error: " + errMsg 
				+ ", sql is: " + command );
	}
	else {
		//Logger::log( STDOUT, INFO, DATABASE, "mysql command executed successfully. sql is: " + command );
	}
	returnConnection( conn );

	return 0;
}

MYSQL_RES *JobLog::query( const string &query ) {
	int ret;
	MYSQL *conn;
	MYSQL_RES *res;
	int ping;


	//if ( !conn ) {
	//Logger::log( STDOUT, ERROR, DATABASE, "mysql connection lost" );
	conn = borrowConnection();

	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Logger::log( STDOUT, ERROR, DATABASE, "mysql ping error in query." );
		return NULL;
	}
	//return NULL;
	//}
	ret = mysql_query( conn, query.c_str() );
	res = mysql_store_result( conn );
	if ( ret != 0 ) {
		string errMsg( mysql_error( conn ) );
		Logger::log( STDOUT, ERROR, DATABASE, "mysql query error: " + errMsg
			   + ", sql is: " + query	);
	}
	else {
		//Logger::log(STDOUT, DEBUG, DATABASE, "mysql query executed successfully. sql is: " + query);
	}
	returnConnection( conn );

	return res;
}

string JobLog::getCurrentTime() {
	const int TIME_LENGTH = 80;
	char result[TIME_LENGTH];
	time_t t = time(0);

	strftime( result, TIME_LENGTH, "%Y-%m-%d %X", localtime( &t ) );

	return string(result);
}
