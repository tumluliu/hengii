/*
 * =====================================================================================
 *
 *       Filename:  mysqljob.cpp
 *
 *    Description:  
 *
 *        Version:  0.7
 *        Created:  04/08/2012 03:11:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani() 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string>
#include "joblog.h"
#include "utility.h"
#include "logger.h"

string JobLog::registerJobSql( int flowId, int jobId, const string &pbsJobId ) {
	return "insert into " + JOB_TABLE_NAME 
		+ "(fid, job_id, pbs_job_id) values('" + Utility::intToString(flowId) 
		+ "', '" + Utility::intToString(jobId) + "', '" + pbsJobId + "');";
}

int JobLog::registerJob( int flowId, int jobId, const string &pbsJobId ) {
	return command(registerJobSql(flowId, jobId, pbsJobId));
}

string JobLog::registerJobFlowSql( int flowId ) {
	return "insert into " + JOB_FLOW_TABLE_NAME + "(id) values('" + Utility::intToString(flowId) + "');";
}

int JobLog::registerJobFlow( int flowId ) {
	return command(registerJobFlowSql(flowId));
}

int JobLog::command( const string &query ) {
	int ret;
	MYSQL *conn;
	conn = mysql_init(NULL);  
	if (!conn) {  
		Logger::log(STDOUT, ERROR, DATABASE, "mysql init failed.");
	}

	conn = mysql_real_connect(conn, hosts.c_str(), userName.c_str(), password.c_str(),dbName.c_str(), port, NULL, 0);  
	if (conn)  
		Logger::log(STDOUT, INFO, DATABASE, "mysql connection successful.");
	else { 
		Logger::log(STDOUT, ERROR, DATABASE, "mysql connection failed.");
		return -1;
	}

	ret = mysql_query(conn, query.c_str()); 
	if (ret != 0) {
		string errMsg(mysql_error(conn));
		Logger::log(STDOUT, ERROR, DATABASE, "mysql query error: " + errMsg);
	} else
		Logger::log(STDOUT, INFO, DATABASE, "mysql query executed successfully.");

	mysql_close(conn);  
	return 0;
}
