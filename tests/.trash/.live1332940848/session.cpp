/*
 * =====================================================================================
 *
 *       Filename:  session.cpp
 *
 *    Description:  implementation of Session class
 *
 *        Version:  0.6
 *        Created:  03/17/2012 11:02:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include "session.h"

Session::Session(): jobThreadIdList(MAX_JOB_COUNT), available(true) { }

Session::Session(int sessionId): jobThreadIdList(MAX_JOB_COUNT), available(true) {
	id = sessionId;
}

int Session::getJobCount() const {
	return jobCount;
}

int Session::getId() const {
	return id;
}

bool Session::isAvailable() const {
	return available;
}

void Session::setAvailable(bool value) {
	available = value;
}

JobTracker Session::getJobTrackerAt(int index) const {
	return jobTrackerList[index];
}

int Session::createJobThreads() {
	for (int i = 0; i < jobCount; i++) {
		int ret;
		if ((ret = pthread_create(&jobThreadIdList[i], 
						&threadAttr, 
						&JobTracker::jobWorker, 
						&jobTrackerList[i])) != 0)
			return ret;
	}
	return 0;
}

void Session::init(const JobFlow& flow) {
	pthread_mutex_init(&threadMutex, NULL);
	pthread_cond_init (&waitingCond, NULL);
	pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
	if (busyParentCountList.empty() == 0) {
		busyParentCountList.clear();
	}
	if (jobTrackerList.empty() == 0) {
		jobTrackerList.clear();
	}
	jobCount = flow.job_count;
	for(int i = 0; i < flow.job_count; i++) {
		busyParentCountList.push_back(flow.jobs[i].parent_count);
		JobTracker job;
		job.setBusyParentCountListIter(busyParentCountList.begin());
		job.setThreadMutex(&threadMutex);
		job.setWaitingCond(&waitingCond);
		job.setUserJob(flow.jobs[i]);
		jobTrackerList.push_back(job);
	}
}

void Session::finalize() {
	pthread_attr_destroy(&threadAttr);
	pthread_mutex_destroy(&threadMutex);
}
