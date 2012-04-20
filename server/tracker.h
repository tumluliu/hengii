/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  declaration of Tracker class
 *
 *        Version:  0.9
 *        Created:  03/17/2012 11:02:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <stdbool.h>
#include "joblog.h"
#include "jobtracker.h"

const int MAX_JOB_COUNT = 20;
const int TRACE_INTERVAL_MILLI_S = 100;

class Tracker{
	private:
		pthread_mutex_t threadMutex;
		pthread_cond_t waitingCond;
		pthread_attr_t threadAttr;
		vector<JobTracker> jobTrackerList;
		vector<int32_t> busyParentCountList;
		vector<pthread_t> jobThreadIdList;
		bool available;
		int jobCount;
		int64_t id;
		string userId;
		JobFlow flow;
		JobLog *log;
		bool isBusy;

	public:
		Tracker();
		Tracker(int64_t);
		~Tracker();
		vector<JobTracker> getJobTrackers() const;
		JobTracker getJobTrackerAt(int) const;
		vector<int32_t> getBusyParentCountList() const;
		void init();
		void setUserJobFlow(const JobFlow&);
		void finalize();
		void setAvailable(bool);
		bool isAvailable() const;
		int createJobThreads();
		int64_t getId() const;
		int getJobCount() const;
		void setUserId(const string&);
		string getUserId() const;
		int trace();
		Status::type getStatus() const;
		Status::type getInnerStatus() const;

		static void* flowWorker(void* threadParam);
};

#endif
