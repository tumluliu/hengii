/*
 * =====================================================================================
 *
 *       Filename:  tracker.cpp
 *
 *    Description:  implementation of Tracker class
 *
 *        Version:  0.8
 *        Created:  03/17/2012 11:02:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tracker.h"
#include "joblog.h"
#include "logger.h"
#include "utility.h"

Tracker::Tracker(): jobThreadIdList(MAX_JOB_COUNT), available(true), isBusy(false) { } // not sure why needed by YANG Anran at 20120414

Tracker::Tracker(int64_t trackerId): jobThreadIdList(MAX_JOB_COUNT), available(true), isBusy(false) {
	id = trackerId;
	log = JobLog::Instance();
}

Tracker::~Tracker() { }

void Tracker::setUserId(const string& uid) {
	userId = uid;
}

string Tracker::getUserId() const {
	return userId;
}

int Tracker::getJobCount() const {
	return jobCount;
}

int64_t Tracker::getId() const {
	return id;
}

bool Tracker::isAvailable() const {
	return available;
}

void Tracker::setAvailable(bool value) {
	available = value;
	if (!available) {
		isBusy = true;
	}
}

JobTracker Tracker::getJobTrackerAt(int index) const {
	return jobTrackerList[index];
}

int Tracker::createJobThreads() {
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

Status::type Tracker::getStatus() const {
	Status::type status = getInnerStatus();

	/* isBusy means the flow needs to do some of his own work despite its jobs */
	if(status == Status::FINISHED || status == Status::FAILED) {
		if (isBusy) {
			status = Status::RUNNING;
		}
	}

	return status;
}

Status::type Tracker::getInnerStatus() const {
	Status::type status = Status::FINISHED;
	for (int i = 0; i < getJobCount(); i++) {
		JobTracker job = getJobTrackerAt(i);
		if (job.getStatus() == JobStatus::FAILED) {
			status = Status::FAILED;
		}	
		if (job.getStatus() != JobStatus::FAILED && job.getStatus() != JobStatus::FINISHED) {
			status = Status::RUNNING;
		}
	}
	return status;
}

int Tracker::trace() {
	Status::type status = Status::RUNNING;

	while (getInnerStatus() != Status::FINISHED && getInnerStatus() != Status::FAILED) {
		status = getInnerStatus();
		log->updateJobFlowStatus(id, status, "");
		usleep(TRACE_INTERVAL_MILLI_S);
	}
}

void Tracker::setUserJobFlow( const JobFlow &jobFlow ) {
	flow = jobFlow;
}

void Tracker::init() {
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

void Tracker::finalize() {
	pthread_attr_destroy(&threadAttr);
	pthread_mutex_destroy(&threadMutex);
	pthread_cond_destroy(&waitingCond);
}

void* Tracker::flowWorker(void* threadParam)
{
	Tracker* flow = (Tracker*) threadParam;

	JobLog::Instance()->registerJobFlow(flow->getId(), flow->userId);
	flow->init();

	if (flow->createJobThreads() != 0) {
		string message = "Create job thread failed. Job flow processing is terminated.";
		Logger::log(STDOUT, ERROR, ENGINE, message);
		JobLog::Instance()->updateJobFlowStatus(flow->getId(), JobStatus::FAILED, message);
	}
	else {
		flow->trace();
		Logger::log(STDOUT, INFO, TORQUE, "flow " + Utility::intToString(flow->getId()) + " finished.");
	}
	flow->isBusy = false;
	JobLog::Instance()->updateJobFlowStatus(flow->id, flow->getStatus(), "");
	return NULL;
}

