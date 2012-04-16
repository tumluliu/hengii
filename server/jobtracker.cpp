/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.cpp
 *
 *    Description:  implementation of JobTracker class
 *
 *        Version:  0.8
 *        Created:  03/17/2012 11:00:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "jobtracker.h"
#include "config.h"
#include "utility.h"
#include "logger.h"
#include "appoption.h"

JobTracker::JobTracker( int64_t id ) {
	flowId = id;
	log = JobLog::Instance();
	outerStatus = JobStatus::NOT_EXIST;
}

void JobTracker::setUserJob(const Job& job) {
	userJob = job;
}

Job JobTracker::getUserJob() const {
	return userJob;
}

string JobTracker::getResult() const {
	return output == "" ? qJob.getOutput() : output;
}

void JobTracker::setResult(const string &result) {
	output = result;
}

string JobTracker::getCmdline() const {
	return qJob.getCmdline();
}

JobStatus::type JobTracker::getStatus() {
	// The inner(pbs) status overwrite outer status when pbsjob is running,
	// but the outer status overwrite the finish state of inner status to
	// do some posting works
	if (outerStatus != JobStatus::RUNNING 
			&& innerStatus != JobStatus::FINISHED) {
		return innerStatus;
	}
	return outerStatus;
}

JobStatus::type JobTracker::updateInnerStatus() {
	char pbsStatus = log->getPbsJobStatus(flowId, userJob.id);

	if (pbsStatus == 'C') {
		innerStatus = JobStatus::FINISHED;
		Logger::log( STDOUT, INFO, ENGINE, 
				"job " + Utility::intToString(userJob.id) + " in flow " + Utility::intToString(flowId) 
				+ " is finished");
		cout << "flow id is: " << flowId << endl;
		cout << "flow id string is: " << Utility::intToString(flowId) << endl;
	}
	else if (pbsStatus == 'Q') {
		innerStatus = JobStatus::QUEUING;
	}
	else if (pbsStatus == 'R') {
		innerStatus = JobStatus::RUNNING;
	}
	else if (pbsStatus == 'S' || pbsStatus == 'H') {
		innerStatus = JobStatus::PAUSED;
	}
	else {
		innerStatus = JobStatus::NOT_EXIST;
	}

	return getStatus();
}

void JobTracker::setStatus(JobStatus::type stat) {
	outerStatus = stat;
	log->updateJobStatus(flowId, userJob.id, stat, qJob.getOutput());
}

int JobTracker::getConnection() const {
	return qJob.getConnection();
}

int64_t JobTracker::getFlowId() const {
	return flowId;
}

string JobTracker::getId() const {
	return qJob.getId();
}

int JobTracker::setProcessCount(int value) {
	return qJob.setProcessCount(value);
}

void JobTracker::setCmdline(const string& value) {
	qJob.setCmdline(value);
}

int JobTracker::submit() {
	return qJob.submit();
}

int JobTracker::collect() {
	int ret = qJob.collect();
	log->updateJobStatus(flowId, userJob.id, getStatus(), qJob.getOutput());
	Logger::log(STDOUT, DEBUG, ENGINE, "message updated is:" + qJob.getOutput());
	return ret;
}

int JobTracker::trace() {
	innerStatus = JobStatus::RUNNING;

	do {
		updateInnerStatus();
		log->updateJobStatus(flowId, userJob.id, getStatus(), "");
		usleep(TRACE_JOB_INTERVAL_MILLI_S);
	} while (innerStatus != JobStatus::FINISHED && innerStatus != JobStatus::FAILED);

	return 0;
}

void JobTracker::setThreadMutex(pthread_mutex_t* mutex) {
	threadMutex = mutex;
}

void JobTracker::setWaitingCond(pthread_cond_t* cond) {
	waitingCond = cond;
}

pthread_mutex_t* JobTracker::getThreadMutex() {
	return threadMutex;
}

pthread_cond_t* JobTracker::getWaitingCond() {
	return waitingCond;
}

vector<int>::iterator JobTracker::getBusyParentCountListIter() {
	return busyParentCountListIter;
}

void JobTracker::setBusyParentCountListIter(vector<int>::iterator it) {
	busyParentCountListIter = it;
}

/*-----------------------------------------------------------------------------
 *  Codes below are the thread function and its partners
 *-----------------------------------------------------------------------------*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  constructCmdOptions
 *  Description:  RT
 * =====================================================================================
 */
string JobTracker::constructCmdOptions ( JobTracker *job, map<string, string> &options, ifstream &metafile ) {
	string result = "";
	string response;
	string record;
	std::vector<string> pieces;
	std::vector<AppOption> appOptionMetas;
	unsigned int i = 0;

	while (getline(metafile, record)) {
		pieces = Utility::splitStringBySpace(record);
		AppOption appOptionMeta(pieces);
		appOptionMetas.push_back(appOptionMeta);
	}

	for (i = 0; i < appOptionMetas.size(); i++) {
		bool lackEssential = true;
		for (map<string, string>::iterator it = options.begin(); it != options.end(); it++) {
			if(appOptionMetas[i].name == (*it).first) {
				result = result + " " + appOptionMetas[i].cmdSwitch + " " + (*it).second;
				lackEssential = false;
			}
		}
		if (appOptionMetas[i].isOptional == false && lackEssential == true) {
			response = "start job error, lack option " + appOptionMetas[i].name;
			Logger::log(STDOUT, ERROR, ENGINE, response);
			job->setStatus(JobStatus::FAILED);
			job->setResult(response);
			cout << "error, flow id " << job->getFlowId() << " id " << job->getUserJob().id;
			JobLog::Instance()->updateJobStatus(job->getFlowId(), job->getUserJob().id, JobStatus::FAILED, response);
			return "";
		}
	}

	for (map<string, string>::iterator it = options.begin(); it != options.end(); it++) {
		if (AppOption::isUnknown(appOptionMetas, (*it).first)) {
			response = "start job error, option " + (*it).first + " unknown";
			Logger::log(STDOUT, ERROR, ENGINE, response);
			job->setStatus(JobStatus::FAILED);
			job->setResult(response);
			JobLog::Instance()->updateJobStatus(job->getFlowId(), job->getUserJob().id, JobStatus::FAILED, response);
			return "";
		}
	}

	return result;
}		/* -----  end of function constructCmdOptions  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jobWorker
 *  Description:  This is the thread function for processing PBS job
 * =====================================================================================
 */
void* JobTracker::jobWorker(void* threadParam)
{
	JobTracker* job = (JobTracker*) threadParam;
	job->setStatus(JobStatus::WAITING_FOR_SUBMIT);
	Job userJob = job->getUserJob();
	string cmdline = "";
	string response = "";
	int processCount;
	ParallelEnv::type parallel_env = userJob.runtime_context.parallel_env;
	map<string, string> envOptions = userJob.runtime_context.options;
	map<string, string> appOptions;

	JobLog::Instance()->registerJob(job->getFlowId(), userJob.id);
	Logger::log(STDOUT, INFO, TORQUE, "job registered.");

	switch (parallel_env) {
		case ParallelEnv::MPI: 
			{
				processCount = atoi(envOptions["process_count"].c_str());
				if (processCount > 1) {
					cmdline += MPI_EXEC_CMD;
					string mpiConf = ""; 
					if (Utility::readFile(MPI_CONF_PATH, mpiConf) != 0) {
						response = "unable to open mpi config file " + MPI_CONF_PATH;
						Logger::log(STDOUT, ERROR, MPI, response);
						job->setStatus(JobStatus::FAILED);
						JobLog::Instance()->updateJobStatus(job->getFlowId(), job->getUserJob().id, JobStatus::FAILED, response);
						return NULL;
					}
					cmdline = cmdline + "-n " + envOptions["process_count"] + " " + mpiConf + " ";
				}
				string appMetaFile = APP_DIR + userJob.app_uri + ".meta";
				ifstream appOptionsFile(appMetaFile.c_str(), ios::in);
				if (!appOptionsFile) { 
					response = "";
					response 
						= response 
						+ "start job error, program "
						+ userJob.app_uri
						+ " not exist";
					Logger::log(STDOUT, ERROR, ENGINE, response);
					job->setResult(response);
					job->setStatus(JobStatus::FAILED);
					JobLog::Instance()->updateJobStatus(job->getFlowId(), job->getUserJob().id, JobStatus::FAILED, response);
					return NULL;
				}
				cmdline = cmdline + APP_DIR + userJob.app_uri + " " + constructCmdOptions(job, userJob.app_options, appOptionsFile);
				if (job->getStatus() == JobStatus::FAILED) {
					return NULL;
				}
				Logger::log(STDOUT, INFO, TORQUE, "The cmdline for PBS is: " + cmdline);
				break;
			}
		case ParallelEnv::OPENMP: break;
		case ParallelEnv::CUDA: break;
		case ParallelEnv::MAPREDUCE: break;
	}
	pthread_mutex_lock(job->threadMutex);
	// Waiting for all the busy parents
	while (*(job->getBusyParentCountListIter() + userJob.id) != 0)
		pthread_cond_wait(job->getWaitingCond(), job->getThreadMutex());
	pthread_mutex_unlock(job->getThreadMutex());

	job->setProcessCount(processCount);
	job->setCmdline(cmdline);
	if (job->submit() != 0) {
		Logger::log(STDOUT, ERROR, TORQUE, "Torque PBS job submission met some problem.");
		Logger::log(STDOUT, INFO, TORQUE, "The process has been terminated. See you next time!");
		Logger::log(STDOUT, INFO, TORQUE, "================================================");
		job->setStatus(JobStatus::FAILED);
	}
	else {
		JobLog::Instance()->registerPbsJob(job->getFlowId(), userJob.id, job->getId());
		Logger::log(STDOUT, INFO, TORQUE, "pbs job registered.");
		job->setStatus(JobStatus::RUNNING);
		Logger::log(STDOUT, INFO, TORQUE, "job start.");
		job->trace();	
		job->collect();
		Logger::log(STDOUT, INFO, TORQUE, "Torque PBS job collects.");
		pthread_mutex_lock(job->getThreadMutex());
		// Tell all the children that I am done
		for (int i = 0; i < userJob.child_count; i++) {
			(*(job->getBusyParentCountListIter() + userJob.children[i]))--;
		}
		pthread_mutex_unlock(job->getThreadMutex());
		pthread_cond_broadcast(job->getWaitingCond());
		Logger::log(STDOUT, INFO, ENGINE, "JobTracker::getResult(): " + job->getResult());
		job->setStatus(JobStatus::FINISHED);
	}
	return NULL;
}

