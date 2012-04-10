#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h> 
}
#define PARAM_SIZE 1024
#define HOST ".localhost.localdomain"
#define HOSTS "localhost"    
#define USERNAME "root"    
#define PASSWORD ""    
#define DBNAME "test"
#define PORT 3306

using namespace std;

class Return_Status{
public:
string Job_Name,ctime,Execution_Time,Error_Path,exec_host,euser,Join_Paths;
vector<string> Resource_List;
string Output_Path,Job_Owner,queue,qtime,job_state,server;
vector<string> Resource_used;
string Variable_List;
};
string update_status(int );
