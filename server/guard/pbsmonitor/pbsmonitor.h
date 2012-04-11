#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h> 
}

using namespace std;

const int PARAM_SIZE     = 1024;
const string HOST_DOMAIN = ".localhost.localdomain";
const string DB_SERVER   = "gdos-yanan";
const string DB_USER     = "myuser";
const string DB_PASSWORD = "mypassword";
const string DB_NAME     = "higis";
const int PORT           = 3306;

class Return_Status {
	public:
		string Job_Name, ctime, Execution_Time, Error_Path, exec_host, euser, Join_Paths;
		vector<string> Resource_List;
		string Output_Path, Job_Owner, queue, qtime, job_state, server;
		vector<string> Resource_used;
		string Variable_List;
};

string update_status(int );
