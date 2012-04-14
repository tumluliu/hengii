/*
 * =====================================================================================
 *
 *       Filename:  joblog.h
 *
 *    Description:  
 *
 *        Version:  0.7
 *        Created:  04/08/2012 03:13:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani
 *         Updator: LIU Lu, YANG Anran
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <mysql/mysql.h>  


#ifndef  JOBLOG_H_
#define  JOBLOG_H_

using namespace std;

const string PBS_JOB_TABLE_NAME = "gdos_sys_pbsjob";
const string JOB_TABLE_NAME = "gdos_sys_job";
const string JOB_FLOW_TABLE_NAME = "gdos_sys_jobflow";
const string DB_SERVER = "127.0.0.1";
const string DB_USER = "myuser";
const string DB_PASSWORD = "mypassword";
const string DB_NAME = "higis";
const int DB_PORT = 3306;
const int MAX_DB_CONN_NUM = 1; // remember to ensure the db max_connections settings is higher than this

class JobLog {
	private:
		string hosts;
		string userName;
		string password;
		string dbName;
		vector<MYSQL*> connPool;
		vector<pthread_mutex_t*> connLock; // true if free
		static JobLog *m_instance;
		int port;

		JobLog();
		~JobLog();
		JobLog(JobLog const&);
		void operator=(JobLog const&);


		MYSQL *createConn();
		string registerPbsJobSql( int64_t, int, const string&);
		string registerJobSql(int64_t, int);
		string registerJobFlowSql(int64_t);
		string updateJobStatusSql(int64_t, int, int, const string&);
		string getPbsJobStatusSql( int64_t, int);
		int command(const string&);
		MYSQL_RES* query( const string & );
		MYSQL* borrowConnection();
		int returnConnection(MYSQL *);
	public:
		int registerJob(int64_t, int, const string&);
		int registerJobFlow(int64_t);
		int updateJobStatus(int64_t, int, int, const string&);
		char getPbsJobStatus( int64_t, int );
		static JobLog *Instance();
};

#endif
