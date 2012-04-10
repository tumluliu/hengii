/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.h
 *
 *    Description:  Declaration of JobTracker class
 *
 *        Version:  0.7 
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

#include <fstream>
#include "lib/HpgcJob.h"
#include "torquejob.h"

using namespace HPGC::HiGIS::Server;

const string MPI_EXEC_CMD = "mpiexec ";

class JobTracker{
	private:
		int flowId;
		pthread_mutex_t* threadMutex;
		pthread_cond_t* waitingCond;
		JobStatus::type jobStatus;
		TorqueJob qJob;
		Job userJob;
		string output; // here to overwrite inner job output in some circumstance, e.g. meta file not found
		vector<int>::iterator busyParentCountListIter;
		static string constructCmdOptions( JobTracker*, map<string, string>&, ifstream& );
	public:
		JobTracker( int );
		void setUserJob( const Job& );
		Job getUserJob() const;
		string getResult() const;
		void setResult(const string &result);
		string getCmdline() const;
		int getConnection() const;
		int submit();
		int collect();
		int setProcessCount(int);
		void setCmdline(const string&);
		int getFlowId() const;
		string getId() const;
		void setThreadMutex(pthread_mutex_t* mutex);
		pthread_mutex_t* getThreadMutex();
		pthread_cond_t* getWaitingCond();
		vector<int>::iterator getBusyParentCountListIter();
		void setBusyParentCountListIter(vector<int>::iterator);
		void setWaitingCond(pthread_cond_t* cond);
		JobStatus::type getStatus() const;
		void setStatus(JobStatus::type);
		static void* jobWorker(void*);
};

#endif
