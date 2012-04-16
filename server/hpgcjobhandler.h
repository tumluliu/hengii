/*
 * =====================================================================================
 *
 *       Filename:  hpgcjobhandler.h
 *
 *    Description:  class declaration of HpgcJobHander
 *
 *        Version:  0.8
 *        Created:  2012年03月23日 11时54分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liu Lu(), nudtlliu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _HPGCJOBHANDLER_H_
#define _HPGCJOBHANDLER_H_

#include <pthread.h>
#include "tracker.h"
#include "utility.h"
#include "config.h"

extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

const string PROJECT_NAME = "HiGIS";
const int TRACKER_POOL_SIZE = 256;
const int JOB_STATUS_FINISHED = 0;
const int JOB_STATUS_UNFINISHED = 1;
const int JOB_STATUS_FAILED= -1;
const int JOB_NOT_EXIST = -2;
const int JOB_STATUS_UNKNOWN = -999;
const int TRACKER_GC_INTERVAL_S = 1;

using namespace HPGC::HiGIS::Server;

class HpgcJobHandler : virtual public HpgcJobIf {
	private:
		map<int64_t, Tracker> trackerPool;
		map<int64_t, Tracker>::const_iterator trackerItr;
		pthread_mutex_t poolLock;
		JobLog *log;
		pthread_t gctId;
		int64_t findEmptyPoolSlot();
		int64_t generateTrackerId();
		void addTracker();
		int createFlowThread( Tracker* );
		static void *cleanWorker(void*);
	public:
		HpgcJobHandler();
		~HpgcJobHandler();
		int64_t start_single_job(const Job& job, const std::string& user_id);
		int64_t start(const JobFlow& flow, const std::string& user_id);
		void pause(const int64_t client_ticket);
		void resume(const int64_t client_ticket);
		void cancel(const int64_t client_ticket);
		void get_status(Result& _return, const int64_t client_ticket);
		void get_my_requests(std::vector<int64_t> & _return, const std::string& user_id); 
		void get_all_requests(std::vector<int64_t> & _return);
};

#endif
