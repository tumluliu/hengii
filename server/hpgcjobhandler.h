/*
 * =====================================================================================
 *
 *       Filename:  hpgcjobhandler.h
 *
 *    Description:  The Entrance of the Hi world
 *
 *        Version:  1.0
 *        Created:  2012年03月23日 11时54分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liu Lu(), nudtlliu@gmail.com
 *                  YANG Anran() 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef HPGCJOBHANDLER_H_
#define HPGCJOBHANDLER_H_

#include <map>
#include <pthread.h>

#include "trackerowner.h"
#include "tracker.h"
#include "config.h"
#include "HpgcJob.h"

/*
 * =====================================================================================
 *        Class:  HpgcJobHandler
 *  Description:  He offers service to the world
 * =====================================================================================
 */
class HpgcJobHandler : public hpgc::higis::interface::HpgcJobIf, public IRecorder
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		HpgcJobHandler();                             /* constructor */
		~HpgcJobHandler();                             /* destructor */

		/* ====================  SERVICE       ======================================= */
		int64_t start_single_job( const hpgc::higis::interface::Job& job, 
				const std::string& user_id);
		int64_t start( const hpgc::higis::interface::JobFlow& flow,
				const std::string& user_id);
		void pause(const int64_t client_ticket);
		void resume(const int64_t client_ticket);
		void cancel(const int64_t client_ticket);
		void get_status( hpgc::higis::interface::Result& _return, 
				const int64_t client_ticket);

		/* ====================  As irecorder ======================================= */
		virtual void OnePlayerDone(int64_t); 

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::map<int64_t, TrackerOwner*> managers_;
		std::map<int64_t, Tracker*> trackers_;
		pthread_mutex_t lock_;

		/* ====================  DISABLED      ======================================= */
		HpgcJobHandler(const HpgcJobHandler &);
		HpgcJobHandler &operator =(const HpgcJobHandler &);
}; /* -----  end of class HpgcJobHandler  ----- */

#endif
