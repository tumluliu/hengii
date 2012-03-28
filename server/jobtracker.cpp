/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.cpp
 *
 *    Description:  implementation of JobTracker class
 *
 *        Version:  0.6
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
#include "jobtracker.h"
#include "config.h"
#include "utility.h"

JobTracker::JobTracker(): threadState(THREAD_STATE_FREE) {
}

void JobTracker::setUserJob(const Job& job) {
	userJob = job;
}

int JobTracker::getThreadState() const {
	return threadState;
}

void JobTracker::setThreadState(int state) {
	threadState = state;
}

Job JobTracker::getUserJob() const {
	return userJob;
}

string JobTracker::getResult() const {
	return qJob.getOutput();
}

string JobTracker::getCmdline() const {
	return qJob.getCmdline();
}

string JobTracker::getStatus() const {
	return qJob.getStatus();
}

int JobTracker::getConnection() const {
	return qJob.getConnection();
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
	return qJob.collect();
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
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jobWorker
 *  Description:  This is the thread function for processing PBS job
 * =====================================================================================
 */
void* JobTracker::jobWorker(void* threadParam)
{
	JobTracker* job = (JobTracker*) threadParam;
	Job userJob = job->getUserJob();
	string cmdline = "";
	int processCount;
	ParallelEnv::type parallel_env = userJob.runtime_context.parallel_env;
	map<string, string> envOptions = userJob.runtime_context.options;
	map<string, string> appOptions;
	switch (parallel_env) {
		case ParallelEnv::MPI: 
			{
				processCount = atoi(envOptions["process_count"].c_str());
				if (processCount > 1) {
					cmdline += MPI_EXEC_CMD;
					string mpiConf = ""; 
					if (Utility::readFile(MPI_CONF_PATH, mpiConf) != 0) {
						cout << "read mpi config file " << MPI_CONF_PATH << " error!" << endl;
						return NULL;
					}
					cmdline = cmdline + "-n " + envOptions["process_count"] + " " + mpiConf + " ";
				}
				string appConfFile = APP_DIR + userJob.app_options["program_name"] + ".meta";
				ifstream appOptionsFile(appConfFile.c_str(), ios::in);
				if (!appOptionsFile) { 
					cout << "open application config file " << appConfFile << " error!" << endl;
					return NULL;
				}
				cmdline = cmdline + APP_DIR + userJob.app_options["program_name"] + " ";
				string appOptionKey;
				appOptions = userJob.app_options;
				while (getline(appOptionsFile, appOptionKey)) {
					cmdline = cmdline + appOptions[appOptionKey] + " ";
				}
				cout << "The cmdline for PBS is: "<< endl;
				cout << cmdline << endl;
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
	cout << "cmdline for qJob: " << cmdline << endl;
	job->setCmdline(cmdline);
	if (job->submit() != 0) {
		cout << "Torque PBS job submission met some problem." << endl;
		cout << "The process has been terminated. See you next time!" << endl;
		cout << "================================================" << endl;
		job->setThreadState(THREAD_STATE_FINISHED_FAILED);
	}
	else {
		job->collect();
		cout << "Torque PBS job collects." << endl;
		pthread_mutex_lock(job->getThreadMutex());
		// Tell all the children that I am done
		for (int i = 0; i < userJob.child_count; i++) {
			(*(job->getBusyParentCountListIter() + userJob.children[i]))--;
		}
		pthread_mutex_unlock(job->getThreadMutex());
		pthread_cond_broadcast(job->getWaitingCond());
		job->setThreadState(THREAD_STATE_FINISHED_SUCCESS);
		cout << "output by invoking JobTracker::getResult() : " << endl;
		cout << job->getResult() << endl;
	}
	pthread_exit(NULL);
}

