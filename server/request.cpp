/*
 * =====================================================================================
 *
 *       Filename:  request.cpp
 *
 *    Description:  implementation of Request class
 *
 *        Version:  0.7
 *        Created:  03/17/2012 11:02:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include "request.h"
#include "joblog.h"

Request::Request(): jobThreadIdList(MAX_JOB_COUNT), available(true) { }

Request::Request(int64_t requestId): jobThreadIdList(MAX_JOB_COUNT), available(true) {
	id = requestId;
}

Request::~Request() { }

void Request::setUserId(const string& uid) {
	userId = uid;
}

string Request::getUserId() const {
	return userId;
}

int Request::getJobCount() const {
	return jobCount;
}

int Request::getId() const {
	return id;
}

bool Request::isAvailable() const {
	return available;
}

void Request::setAvailable(bool value) {
	available = value;
}

JobTracker Request::getJobTrackerAt(int index) const {
	return jobTrackerList[index];
}

int Request::createJobThreads() {
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

void Request::init(const JobFlow& flow) {
	JobLog::Instance()->registerJobFlow(id);

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
		JobTracker job(id);
		job.setBusyParentCountListIter(busyParentCountList.begin());
		job.setThreadMutex(&threadMutex);
		job.setWaitingCond(&waitingCond);
		job.setUserJob(flow.jobs[i]);
		jobTrackerList.push_back(job);
	}
}

void Request::finalize() {
	pthread_attr_destroy(&threadAttr);
	pthread_mutex_destroy(&threadMutex);
}
