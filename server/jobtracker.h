/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.h
 *
 *    Description:  Declaration of JobTracker class
 *
 *        Version:  0.6 
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

#include "lib/HpgcJob.h"
#include "torquejob.h"

using namespace HPGC::HiGIS::Server;

const string MPI_EXEC_CMD = "mpiexec ";
const int THREAD_STATE_FINISHED_SUCCESS = 0;
const int THREAD_STATE_FINISHED_FAILED = -1;
const int THREAD_STATE_BUSY = 1;
const int THREAD_STATE_FREE = 2;

class JobTracker{
	private:
		pthread_mutex_t* threadMutex;
		pthread_cond_t* waitingCond;
		int threadState;
		TorqueJob qJob;
		Job userJob;
		vector<int>::iterator busyParentCountListIter;
	public:
		JobTracker();
		void setUserJob(const Job&);
		Job getUserJob() const;
		string getResult() const;
		string getCmdline() const;
		string getStatus() const;
		int getConnection() const;
		int submit();
		int collect();
		int setProcessCount(int);
		void setCmdline(const string&);
		string getId() const;
		void setThreadMutex(pthread_mutex_t* mutex);
		pthread_mutex_t* getThreadMutex();
		pthread_cond_t* getWaitingCond();
		vector<int>::iterator getBusyParentCountListIter();
		void setBusyParentCountListIter(vector<int>::iterator);
		void setWaitingCond(pthread_cond_t* cond);
		int getThreadState() const;
		void setThreadState(int);
		static void* jobWorker(void*);
};

#endif
