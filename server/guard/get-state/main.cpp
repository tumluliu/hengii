#include "get_state.h"
#include <mysql/mysql.h>  
#include <sys/param.h>
#include <sys/stat.h>
string Query(string que) {
	int  t, r;
	string query;
	string result ;
	MYSQL  *conn_ptr;
	MYSQL_RES  *res;
	MYSQL_ROW  row;

	conn_ptr = mysql_init(NULL);  

	if (!conn_ptr) {  
		fprintf(stderr, "mysql_init failed\n");

	}
	conn_ptr = mysql_real_connect(conn_ptr, HOSTS, USERNAME, PASSWORD, DBNAME, PORT, NULL, 0);  
	if (conn_ptr)  
		cout<<"Connection success"<<endl;  
	else  
		cout<<"Connection failed"<<endl;  

	query = que;
	t = mysql_query(conn_ptr, query.c_str()); 
	if (t)
		cout<<"Error making query: "<< mysql_error(conn_ptr)<<endl;
	else
		cout<<"Query made..."<<endl;
	res=mysql_store_result(conn_ptr);
	if (res!=NULL)
	{
		for(r = 0; r <= mysql_field_count(conn_ptr); r++)
		{
			row = mysql_fetch_row(res);
			if(row == NULL)
				break;

			for(t = 0; t < mysql_num_fields(res); t++)
			{	
				string re = row[t];
				result= result + re ;				
			}
		}	
		cout<<result<<endl; 	
	}
	mysql_free_result(res);
	mysql_close(conn_ptr);  
	return result;
}

int GetId(string Job_id)
{
	string::size_type pos=Job_id.find(".");
	string Jobid(Job_id,0,pos);
	int id = atoi(Jobid.c_str());
	return id;
}

void init_daemon(void)
{
	int pid;
	int i;
	if(pid=fork())
		exit(0);
	else if(pid< 0)
	{
		exit(1);
	}
	setsid();
	if(pid=fork())
		exit(0);
	else if(pid< 0)
	{
		exit(1);
	}

	for(i=0;i< NOFILE;++i)
		close(i);
	umask(0);
	return;
}

int main(int argc,char** argv)
{
	//	init_daemon();
	do
	{		
		string que = "select job_id from jobstatus order by id desc limit 1;";
		string Jobid = Query(que);
		int Job_id = GetId(Jobid);
		cout<<"Job_id = "<<Job_id<<endl;
		string sql = update_status(Job_id);
		while (sql.compare("")!=0)
		{
			string result = Query(sql);
			Job_id--;
			sql = update_status(Job_id);
		}
	}while(true);
	return 1;

}

