/*
 * =====================================================================================
 *
 *       Filename:  mysqljob.h
 *
 *    Description:  
 *
 *        Version:  0.7
 *        Created:  04/08/2012 03:13:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mysql/mysql.h>  

using namespace std;

const string JOB_TABLE_NAME = "gdos_sys_job";
const string JOB_FLOW_TABLE_NAME = "gdos_sys_jobflow";
const string DB_SERVER = "gdos-yanan";
const string DB_USER = "myuser";
const string DB_PASSWORD = "mypassword";
const string DB_NAME = "higis";
const int DB_PORT = 3306;

class JobLog {
	private:
		string hosts;
		string userName;
		string password;
		string dbName;
		int port;
		string registerJobSql(int, int, const string&);
		string registerJobFlowSql(int);
		int command(const string&);
	public:
		JobLog(): hosts(DB_SERVER),userName(DB_USER),password(DB_PASSWORD),dbName(DB_NAME),port(DB_PORT){ }
		int registerJob(int, int, const string&);
		int registerJobFlow(int);
};
