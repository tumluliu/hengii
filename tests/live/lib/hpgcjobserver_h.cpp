#include "hpgcjobserver_h.h"

 int findAvailableResource(JobflowParam* resources)
{
	int i;
	for(i = 0; i < 16; i++)
	{
		if(resources[i].available != 9)   //资源可用
			return i;
	}
	return -1;
}
 

string readFileToString(string file)
{
	string content;
	ifstream infile(file.c_str(),ios::in);
	if (!infile)
		cout<<"open file "<<file<<" error!"<<endl;
	content.clear();
	copy(istreambuf_iterator<char>(infile),istreambuf_iterator<char>(),back_inserter(content));
	if (isspace(content[content.size()-1]))	
		content[content.size()-1]=' ';
	infile.close();
//	cout<<content<<endl;
	return content;
}

void hpgcLog(char *file,char *str)
{
	FILE *fp;
	time_t t;
	fp=fopen(file,"a");
	if((fp) >=0)
	 {
	 	 t=time(0);
		 fprintf(fp,"%s:%s\n",asctime(localtime(&t)),str );
		 fclose(fp);
	  }
}

void submitPbs(PbsParam* Pbs_Param)
{
	string cmdline = Pbs_Param->cmdline;

 	int rnd;
//	 	FILE *Tfp,*fp;
   char script[11];
   char stdout[]="pbs";
	char error[]="error.log";
	char dir[]="/";
	char proc[2];

   char errPath[PARAM_SIZE];
	char Path[PARAM_SIZE];
	char Hostname[PARAM_SIZE];
  	memset(Pbs_Param->scrPath, '\0', sizeof(Pbs_Param->scrPath) ); 
 	memset(Pbs_Param->stdPath, '\0', sizeof(Pbs_Param->stdPath) );  
	memset(errPath, '\0', sizeof(errPath) );
	memset(script, '\0', sizeof(script) );
	memset(Path, '\0', sizeof(Path) ); 
	memset(Hostname, '\0', sizeof(Hostname) );  

	getcwd(Path,sizeof(Path));
	strcat(Path,dir);

	struct timeval tpstart;
	gettimeofday(&tpstart,NULL);
	srand(tpstart.tv_usec);
	rnd=rand();
	sprintf(script,"%d",rnd);
	strcat(Pbs_Param->scrPath,Path);
	strcat(Pbs_Param->scrPath,script);
	strcat(Pbs_Param->stdPath,Path);
	strcat(Pbs_Param->stdPath,stdout);
	strcat(Pbs_Param->stdPath,script);
	strcat(errPath,Path);
	strcat(errPath,error);
//		cout<<"scrPath:"<<scrPath<<endl;
//		cout<<"stdPath:"<<stdPath<<endl;
//		cout<<"errPath:"<<errPath<<endl;
	int exist;
	char str3[]="0";
	while((exist=access(Pbs_Param->scrPath,0))==0)
	{
		printf("%s exist!\n",Pbs_Param->scrPath);
		strcat(script,str3);
		strcat(Pbs_Param->scrPath,str3);
		strcat(Pbs_Param->stdPath,str3);	    	
	}
	ofstream outfile(Pbs_Param->scrPath,ios::out);
	if (!outfile)
		cout<<"open file error!"<<endl;
	outfile<<cmdline<<endl;
	outfile.close();
	sprintf(proc,"%d",Pbs_Param->procs);

	gethostname(Hostname,sizeof(Hostname));
	int Con=pbs_connect(Hostname); //连结服务器”server”
	Pbs_Param->connect=Con;
	struct attropl attrib[4];

	attrib[0].name = ATTR_N;
	attrib[0].value = script;
	attrib[0].resource='\0';
	attrib[0].next=&attrib[1];
	attrib[1].name=ATTR_o;
	attrib[1].value=Pbs_Param->stdPath;
	attrib[1].resource='\0';
	attrib[1].next=&attrib[2];
	attrib[2].name=ATTR_e;
	attrib[2].value=errPath;
	attrib[2].resource='\0';
	attrib[2].next=&attrib[3];
	attrib[3].name=ATTR_l;
	attrib[3].value=proc;
	attrib[3].resource="procs";
	attrib[3].next=NULL;
		
	char *Ret=pbs_submit(Con,attrib,Pbs_Param->scrPath,0,0);//提交任务
	if (!Ret)
	{
		printf("ERR=%d\n",pbs_errno);//发生错误，打印错误代码
		printf( "error[%s]", strerror( pbs_errno ) );
	}
/*	else
	{
		cout<<"Ret="<<Ret<<endl;//打印返回的任务标识
	}*/
	Pbs_Param->pbsjob_id=Ret;
	cout<<"pbsjob_id="<<Pbs_Param->pbsjob_id<<endl;
	free(Ret);
}

string returnPbs(PbsParam* Pbs_Param)
{
 	string logBuf;
	char* pbsjob_id;
	int Con;
	pbsjob_id=const_cast<char*>((Pbs_Param->pbsjob_id).c_str()) ;
	Con = Pbs_Param->connect;
	char value[] = "\0";
	char resource[] = "\0";
	struct attrl attr;
	attr.name = ATTR_state;
	attr.value = value;
	attr.resource=resource;
	attr.next=NULL;

	char *state;
	struct batch_status *status;
	do
	{
		do {
		 status=pbs_statjob(Con,pbsjob_id,&attr,0);
	   // sleep(1);
		} while(status == NULL);
		 state=(status->attribs)->value;	 
	  Pbs_Param->pbs_state=*state;
	}while(*state!='C');
	logBuf=readFileToString(Pbs_Param->stdPath);
	string sendData=logBuf;

 	char del[PARAM_SIZE]="rm ";
 	char str2[]=" ";
 	strcat(del,Pbs_Param->scrPath);
 	strcat(del,str2);
 	strcat(del,Pbs_Param->stdPath);
//	 	cout<<"del:"<<del<<endl;
 	int result=system(del);
	/*   if(result!=0)
	        {
		}*/
//	free(pbsjob_id);
	pbs_statfree(status);
	pbs_disconnect(Con);
	return sendData;
}

void* threadFunction(void* Param)
{
	struct ThreadParam* params=(struct ThreadParam*) Param;
	Job* job = &(params->job);
	PbsParam Pbs_Param;
	string cmdline;
	string sendData;
//	 	char str1[]="$";
 	int jobID,parent_count,childcount;
	int procs,i;
	char Path[PARAM_SIZE];	
	char dir[]="/";
	memset(Path, '\0', sizeof(Path) ); 
	
	
	getcwd(Path,sizeof(Path));
	strcat(Path,dir);
	
 	jobID=job->id;
 	parent_count=job->parent_count;
	childcount=job->child_count;
	int child[childcount];
	for(i=0;i<childcount;i++)
	{
		child[i]=job->children[i];
	}
	ParallelEnv::type parallel_env = job->runtime_context.parallel_env;
	//	cout<<parallel_env<<endl;
  	map<string,string> options=job->runtime_context.options ;
	map<string,string>::iterator it = options.find("process_count");
	map<string,string> app_options;
	map<string,string>::iterator iter;
	string conf_path;
	char proc[2];
	string conf;
	ifstream infile( "app_map.conf",ios::in);
	if (!infile)
	cout<<"open app_map.conf file error!"<<endl;
	switch(parallel_env)
	{
		case 1:
 
		if(it !=options.end())
			procs=atoi(it->second.c_str());
		sprintf(proc,"%d",procs);
	
	 	if(procs!=1)
		{
			conf_path=Path;
 			conf_path+="hpgcserver.conf";
			cmdline="mpiexec -n ";
			cmdline=cmdline+proc+" "+readFileToString(conf_path);
      		}
	//	cout<<"cmdline"<<cmdline<<endl;
		while(getline(infile,conf))
		{
	  		app_options=job->app_options;
			iter= app_options.find(conf);
			cmdline=cmdline+" "+iter->second;
		}
		cout<<"cmdline"<<cmdline<<endl;
		break;

		case 2:break;
		case 3:break;
		case 4:break;
	}
/*	    if(params->state_GLOBAL[jobID]<0)
	 	{
	      hpgcLog("error.log","error reliance!");
	 	    sendData="Run failed:error reliance!";
	 	    goto loop;
	 	}
*/
//	cout<<"state_global is:"<<*(params->state_global_iter+jobID)<<endl;
 	pthread_mutex_lock(params->state_mutex_GLOBAL);//加锁
 	while(*(params->state_global_iter+jobID)!=0)//等到parent_count==0
		pthread_cond_wait(params->state_threshold_cv_GLOBAL, params->state_mutex_GLOBAL);//等待
 	pthread_mutex_unlock(params->state_mutex_GLOBAL);//解锁

 	//执行计算作业====================================================================================
	
	Pbs_Param.procs = procs;	
	Pbs_Param.cmdline = cmdline;
	submitPbs(&Pbs_Param);
	
	params->pbs_state = &(Pbs_Param.pbs_state);	
	params->connect = Pbs_Param.connect;
	params->pbsjob_id = Pbs_Param.pbsjob_id;

	sendData = returnPbs(&Pbs_Param);

	pthread_mutex_lock(params->state_mutex_GLOBAL);//加锁
	for(i=0;i<childcount;i++)
	{
		int childID=child[i];
		(*(params->state_global_iter+childID))--;//将child的parent_count减1
	}
	pthread_mutex_unlock(params->state_mutex_GLOBAL);//解锁
	pthread_cond_broadcast(params->state_threshold_cv_GLOBAL);//广播

//		loop:params->result[jobID].clear();
	params->result.clear();
	params->result=sendData;
	params->thread_state=1;
//		cout<<"The sendData is:"<<	params->result<<endl;
	pthread_exit(NULL);
 }

