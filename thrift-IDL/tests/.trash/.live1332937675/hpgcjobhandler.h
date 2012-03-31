/*
 * =====================================================================================
 *
 *       Filename:  hpgcjobhandler.h
 *
 *    Description:  class declaration of HpgcJobHander
 *
 *        Version:  0.6
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

#include "session.h"
#include "utility.h"
#include "config.h"

extern "C"
{
#include <pbs_error.h>
#include <pbs_ifl.h>
}

const string PROJECT_NAME = "HiGIS";
const int SESSION_POOL_SIZE = 16;
const int JOB_STATUS_FINISHED = 0;
const int JOB_STATUS_UNFINISHED = 1;
const int JOB_STATUS_FAILED= -1;
const int JOB_NOT_EXIST = -2;

using namespace HPGC::HiGIS::Server;

class HpgcJobHandler : virtual public HpgcJobIf {
	private:
		map<int, Session> sessionPool;
		map<int, Session>::const_iterator sessionItr;
		int findEmptyPoolSlot();
		int generateSessionId();
	public:
		HpgcJobHandler();
		int32_t start_single_job(const Job& job);
		int32_t start(const JobFlow& flow);
		void pause(const int32_t client_ticket);
		void resume(const int32_t client_ticket);
		void cancel(const int32_t client_ticket);
		void get_status(Result& _return, const int32_t client_ticket);
};

#endif
