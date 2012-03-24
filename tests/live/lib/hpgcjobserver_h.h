#include "HpgcJob.h"
#include <iostream>
#include <fstream>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TNonblockingServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <string>
#include <vector>
extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

#define PARAM_SIZE 1024

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace HPGC::HiGIS::Server;
using namespace std;

class ThreadParam{
  public:
	Job job;
	pthread_mutex_t* state_mutex_GLOBAL;
	pthread_cond_t* state_threshold_cv_GLOBAL;
	vector<int>::iterator state_global_iter;
	string result;
	string pbsjob_id;
	int connect;
	int thread_state;
	char* pbs_state;
};

class JobflowParam{
  public:
	vector<ThreadParam> Threadparam;
	vector<int> state_global;
	pthread_t threadId_GLOBAL[20];
	pthread_mutex_t state_mutex_global;
	pthread_cond_t state_threshold_cv_global;
	pthread_attr_t attr;
	int available;
};

class PbsParam{
  public:
	string cmdline;
	int procs;
	char scrPath[PARAM_SIZE];
   char stdPath[PARAM_SIZE];
	string pbsjob_id;
	int connect;
	char pbs_state;
};

 int findAvailableResource(JobflowParam* );

 string readFileToString(string );

 void hpgcLog (char *,char *);

 void* threadFunction(void* );

