/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  declaration of Session class
 *
 *        Version:  0.7
 *        Created:  03/17/2012 11:02:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _SESSION_H_
#define _SESSION_H_

#include "jobtracker.h"

const int MAX_JOB_COUNT = 20;

class Session{
	private:
		pthread_mutex_t threadMutex;
		pthread_cond_t waitingCond;
		pthread_attr_t threadAttr;
		vector<JobTracker> jobTrackerList;
		vector<int> busyParentCountList;
		vector<pthread_t> jobThreadIdList;
		bool available;
		int jobCount;
		int id;
	public:
		Session();
		Session(int);
		vector<JobTracker> getJobTrackers() const;
		JobTracker getJobTrackerAt(int) const;
		vector<int> getBusyParentCountList() const;
		void init(const JobFlow&);
		void finalize();
		void setAvailable(bool);
		bool isAvailable() const;
		int createJobThreads();
		int getId() const;
		int getJobCount() const;
};

#endif
