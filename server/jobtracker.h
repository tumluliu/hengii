/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.h
 *
 *    Description:  Declaration of JobTracker class
 *
 *        Version:  0.8 
 *        Created:  03/17/2012 10:59:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _JOBTRACKER_H_
#define _JOBTRACKER_H_

#include <stdint.h>
#include <fstream>
#include <pthread.h>
#include "lib/HpgcJob.h"
#include "torquejob.h"
#include "joblog.h"

using namespace HPGC::HiGIS::Server;

const string MPI_EXEC_CMD = "mpiexec ";
const int TRACE_JOB_INTERVAL_MILLI_S = 100;

class JobTracker{
	private:
		int64_t flowId;
		pthread_mutex_t* threadMutex;
		pthread_cond_t* waitingCond;
		JobStatus::type innerStatus;
		JobStatus::type outerStatus; // the status determined by the job but not determined by TorqueJob
		TorqueJob qJob;
		Job userJob;
		JobLog *log;
		string output; // here to overwrite inner job output in some circumstance, e.g. meta file not found
		vector<int>::iterator busyParentCountListIter;
		static string constructCmdOptions( JobTracker*, map<string, string>&, ifstream& );
	public:
		JobTracker( int64_t );
		void setUserJob( const Job& );
		Job getUserJob() const;
		string getResult() const;
		void setResult(const string &result);
		string getCmdline() const;
		int getConnection() const;
		int submit();
		int collect();
		int trace();
		int setProcessCount(int);
		void setCmdline(const string&);
		int64_t getFlowId() const;
		string getId() const;
		void setThreadMutex(pthread_mutex_t* mutex);
		pthread_mutex_t* getThreadMutex();
		pthread_cond_t* getWaitingCond();
		vector<int>::iterator getBusyParentCountListIter();
		void setBusyParentCountListIter(vector<int>::iterator);
		void setWaitingCond(pthread_cond_t* cond);
		JobStatus::type getStatus();
		JobStatus::type updateInnerStatus();
		void setStatus(JobStatus::type, const string &);
		void setStatus(JobStatus::type);
		static void* jobWorker(void*);
};

#endif
