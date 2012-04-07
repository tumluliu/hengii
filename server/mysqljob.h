#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mysql/mysql.h>  

using namespace std;

class MysqlJob{
	private:
		string sql;
		string mysqlResult;
		string hosts;
		string userName;
		string password;
		string dbName;
		int port;
	public:
		MysqlJob(): mysqlResult(""),sql(""),hosts("localhost"),userName("root"),password(""),dbName("test"),port(3306){ }
		void setSql(string);
		void Query ();
		string getSqlResult();
};
