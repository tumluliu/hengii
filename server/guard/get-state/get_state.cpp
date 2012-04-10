#include "get_state.h"


string getdate(long time) {
	string date;
	if (time==0)
	{
		date = "";
	}
	else
	{	
		ostringstream os;
		long ss = time + 8 * 60 * 60;// 秒数
		os<<(ss % 60);
		string s=os.str();
		//	cout<<"秒：" << s<< endl;
		long mm = ss / 60;
		os.str("");
		os<<(mm % 60);
		string m=os.str();
		//	cout<<"分：" << m << endl;
		long hh = mm / 60;
		os.str("");
		os<<(hh % 24);
		string h=os.str();
		//	cout<<"时：" << h << endl;
		long dd = hh / 24;
		int year = 1970;// 年
		long day = 0;
		for (; true; year++) {
			if (year % 4 == 0) {
				day += 366;
			} else {
				day += 365;
			}
			if (day > dd) {
				break;
			}
		}
		if (year % 4 == 0) {
			dd -= (day - 366);
		} else {
			dd -= (day - 365);
		}
		day = 0;
		long old_day = 0;
		int MM = 1;// 月
		for (; MM < 13; MM++) {
			old_day = day;
			switch (MM) {
				case 1:
					day += 31;
					break;
				case 2:
					if (year % 4 == 0) {
						day += 29;
					} else {
						day += 28;
					}
					break;
				case 3:
					day += 31;
					break;
				case 4:
					day += 30;
					break;
				case 5:
					day += 31;
					break;
				case 6:
					day += 30;
					break;
				case 7:
					day += 31;
					break;
				case 8:
					day += 31;
					break;
				case 9:
					day += 30;
					break;
				case 10:
					day += 31;
					break;
				case 11:
					day += 30;
					break;
				case 12:
					day += 31;
					break;
			}
			if (day > dd) {
				break;
			}
		}
		dd -= old_day;
		dd++;

		os.str("");
		os<<year ;
		string y=os.str();
		os.str("");
		os<<MM;
		string mo = os.str();
		os.str("");
		os<<dd;
		string d = os.str();
		date= y+"-" + mo + "-" + d + " " + h + ":"+ m + ":" + s;
		//cout<<year << "-" << MM << "-" << dd << " " << (hh % 24) << ":"<< (mm % 60) << ":" << (ss % 60);
	}	
	return date;
}



string update_status(int id){
	char Hostname[PARAM_SIZE];
	gethostname(Hostname,sizeof(Hostname));
	int Con=pbs_connect(Hostname); 
	printf("Con=%d\n",Con);

	char  jobid[PARAM_SIZE];
	sprintf(jobid,"%d",id);
	strcat(jobid,HOST);
	string job_id = jobid;

	struct attrl attr[16];

	attr[0].name      = ATTR_N;
	attr[0].value     = NULL;
	attr[0].resource  = NULL;
	attr[0].next      = &attr[1];
	attr[1].name      = ATTR_ctime;
	attr[1].value     = NULL;
	attr[1].resource  = NULL;
	attr[1].next      = &attr[2];
	attr[2].name      = ATTR_etime;
	attr[2].value     = NULL;
	attr[2].resource  = NULL;
	attr[2].next      = &attr[3];
	attr[3].name      = ATTR_e;
	attr[3].value     = NULL;
	attr[3].resource  = NULL;
	attr[3].next      = &attr[4];
	attr[4].name      = ATTR_exechost;
	attr[4].value     = NULL;
	attr[4].resource  = NULL;
	attr[4].next      = &attr[5];
	attr[5].name      = ATTR_euser;
	attr[5].value     = NULL;
	attr[5].resource  = NULL;
	attr[5].next      = &attr[6];
	attr[6].name      = ATTR_j;
	attr[6].value     = NULL;
	attr[6].resource  = NULL;
	attr[6].next      = &attr[7];
	attr[7].name      = ATTR_l;
	attr[7].value     = NULL;
	attr[7].resource  = NULL;
	attr[7].next      = &attr[8];
	attr[8].name      = ATTR_o;
	attr[8].value     = NULL;
	attr[8].resource  = NULL;
	attr[8].next      = &attr[9];
	attr[9].name      = ATTR_owner;
	attr[9].value     = NULL;
	attr[9].resource  = NULL;
	attr[9].next      = &attr[10];
	attr[10].name     = ATTR_queue;
	attr[10].value    = NULL;
	attr[10].resource = NULL;
	attr[10].next     = &attr[11];
	attr[11].name     = ATTR_qtime;
	attr[11].value    = NULL;
	attr[11].resource = NULL;
	attr[11].next     = &attr[12];
	attr[12].name     = ATTR_state;
	attr[12].value    = NULL;
	attr[12].resource = NULL;
	attr[12].next     = &attr[13];
	attr[13].name     = ATTR_server;
	attr[13].value    = NULL;
	attr[13].resource = NULL;
	attr[13].next     = &attr[14];
	attr[14].name     = ATTR_used;
	attr[14].value    = NULL;
	attr[14].resource = NULL;
	attr[14].next     = &attr[15];
	attr[15].name     = ATTR_v;
	attr[15].value    = NULL;
	attr[15].resource = NULL;
	attr[15].next     = NULL;

	struct batch_status *sta=pbs_statjob(Con,jobid,attr,0);
	string sql_update;

	if (sta == NULL)
		sql_update="";
	else
	{
		Return_Status status;
		struct attrl *attr_runtime;
		string resource_list;
		string resource_used;
		//	string _ctime;
		attr_runtime = sta->attribs;
		/*for(int i=0;i<16;i++)
		  {
		  cout<<attr_runtime->name<<endl;
		  attr_runtime = attr_runtime->next;
		  }*/
		if (strcmp(attr_runtime->name,"Job_Name")==0)
		{
			status.Job_Name = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.Job_Name = "";

		if (strcmp(attr_runtime->name,"ctime")==0)
		{
			long c_time = atoi(attr_runtime->value);
			status.ctime = getdate(c_time);
			attr_runtime = attr_runtime->next;
		}
		else
			status.ctime = "";

		if (strcmp(attr_runtime->name,"etime")==0)
		{		
			long e_time = atoi(attr_runtime->value);
			status.Execution_Time = getdate(e_time);
			attr_runtime = attr_runtime->next;
		}
		else
			status.Execution_Time = "";

		if (strcmp(attr_runtime->name,"Error_Path")==0)
		{
			status.Error_Path = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.Error_Path = "";

		if (strcmp(attr_runtime->name,"exec_host")==0)
		{
			status.exec_host = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.exec_host = "";

		if (strcmp(attr_runtime->name,"euser")==0)
		{
			status.euser = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.euser = "";

		if (strcmp(attr_runtime->name,"Join_Paths")==0)
		{
			status.Join_Paths = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.Join_Paths = "";

		resource_list = "<xml>";
		while (strcmp(attr_runtime->name,"Resource_List")==0)
		{
			cout<<"resource"<<endl;
			resource_list = resource_list + "<a v=\"";
			string resource = attr_runtime->resource;
			string value = attr_runtime->value;
			resource_list = resource_list + resource + "\">" + value + "</a>";
			attr_runtime = attr_runtime->next;
		}
		resource_list = resource_list + "</xml>";

		if (strcmp(attr_runtime->name,"Output_Path")==0)
		{
			status.Output_Path = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.Output_Path = "";

		if (strcmp(attr_runtime->name,"Job_Owner")==0)
		{
			status.Job_Owner = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.Job_Owner = "";

		if (strcmp(attr_runtime->name,"queue")==0)
		{
			status.queue = attr_runtime->value;
			attr_runtime = attr_runtime->next;
		}
		else
			status.queue = "";

		if (strcmp(attr_runtime->name,"qtime")==0)
		{
			long q_time = atoi(attr_runtime->value);
			status.qtime = getdate(q_time);
			attr_runtime = attr_runtime->next;	
		}
		else
			status.qtime = "";

		if (strcmp(attr_runtime->name,"job_state")==0)
		{
			status.job_state = attr_runtime->value;
			attr_runtime = attr_runtime->next;	
		}
		else
			status.job_state = "";

		if (strcmp(attr_runtime->name,"server")==0)
		{
			status.server = attr_runtime->value;
			attr_runtime = attr_runtime->next;	
		}
		else
			status.server = "";

		resource_used = "<xml>";
		while (strcmp(attr_runtime->name,"Resource_used")==0)
		{
			cout<<"resource"<<endl;
			resource_used = resource_used + "<a v=\"";
			string resource = attr_runtime->resource;
			string value = attr_runtime->value;
			resource_used = resource_used + resource + "\">" + value + "</a>";
			attr_runtime = attr_runtime->next;
		}
		resource_used = resource_used + "</xml>";

		if (strcmp(attr_runtime->name,"Variable_List")==0)
			status.Variable_List = attr_runtime->value;
		else
			status.Variable_List = "";


		sql_update = "update jobstatus set job_name='" + status.Job_Name + "',ctime='" + status.ctime + "',execution_time='" + status.Execution_Time + "',error_path='" + status.Error_Path + "',exec_host='" + status.exec_host + "',euser='" + status.euser + "',join_paths='" + status.Join_Paths + "',resource_list='" + resource_list + "',output_path='" + status.Output_Path + "',job_owner='" + status.Job_Owner + "',queue='" + status.queue + "',qtime='" + status.qtime + "',job_state='" + status.job_state + "',server='" + status.server + "',resource_used='" + resource_used +  "' where job_id='" + job_id +"';"; 


		cout<<sql_update <<endl;
	}

	pbs_statfree(sta);

	pbs_disconnect(Con);
	return sql_update;
}
