/*
 * =====================================================================================
 *
 *       Filename:  hpgcjobhandler.h
 *
 *    Description:  class declaration of HpgcJobHander
 *
 *        Version:  0.7
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

#include "request.h"
#include "utility.h"
#include "config.h"

extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

const string PROJECT_NAME = "HiGIS";
const int REQUEST_POOL_SIZE = 256;
const int JOB_STATUS_FINISHED = 0;
const int JOB_STATUS_UNFINISHED = 1;
const int JOB_STATUS_FAILED= -1;
const int JOB_NOT_EXIST = -2;
const int JOB_STATUS_UNKNOWN = -999;

using namespace HPGC::HiGIS::Server;

class HpgcJobHandler : virtual public HpgcJobIf {
	private:
		map<int64_t, Request> requestPool;
		map<int64_t, Request>::const_iterator requestItr;
		int64_t findEmptyPoolSlot();
		int64_t generateRequestId();
		void addRequest();
	public:
		HpgcJobHandler();
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
