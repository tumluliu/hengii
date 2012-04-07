#include "mysqljob.h"

void MysqlJob::setSql(string id){
	sql = "insert into jobstatus(job_id) values('" + id + "');";
}

string  MysqlJob::getSqlResult(){
	return mysqlResult;
}

void MysqlJob::Query() {
	int  t, r;
	string query;
	MYSQL  *conn_ptr;
	MYSQL_RES  *res;
	MYSQL_ROW  row;
	conn_ptr = mysql_init(NULL);  
	if (!conn_ptr) {  
		fprintf(stderr, "mysql_init failed\n");
	}

	conn_ptr = mysql_real_connect(conn_ptr, hosts.c_str(), userName.c_str(), password.c_str(),dbName.c_str(), port, NULL, 0);  
	if (conn_ptr)  
		cout<<"Connection success"<<endl;  
	else  
		cout<<"Connection failed"<<endl;  

	query = sql;
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
				mysqlResult= mysqlResult + re ;				
			}
		}	
		cout<<mysqlResult<<endl; 	
	}
	mysql_free_result(res);
	mysql_close(conn_ptr);  

}
