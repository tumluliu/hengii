/*
 * =====================================================================================
 *
 *       Filename:  trackercenter.h
 *
 *    Description:  The manager of all trackers
 *
 *        Version:  1.0
 *        Created:  05/17/2012 08:10:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  TRACKERCENTER_H_
#define  TRACKERCENTER_H_

#include "trackerfactory.h"
#include "tracker.h"
#include "jobrepo.h"

/*
 * =====================================================================================
 *        Class:  TrackerCenter
 *  Description:  He is initially to deal with the conflict of boost::shared_ptr
 *                and std::shared_ptr. But this splition of interface and implementation
 *                is expected since long ago.
 * =====================================================================================
 */
class TrackerCenter : public IRecorder, public std::enable_shared_from_this<TrackerCenter>
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		TrackerCenter(JobRepo &);                             /* constructor */
		~TrackerCenter();                             /* destructor */

		/* ====================  As irecorder ======================================= */
		virtual void OnePlayerDone(int64_t); 

		/* ====================  MUTATORS      ======================================= */
		/* add a tracker created with Tracker Factory, return id */
		void add_tracker(std::shared_ptr<Tracker>);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		JobRepo &repo_;
		std::map<int64_t, std::shared_ptr<Tracker> > trackers_;
		pthread_mutex_t lock_;

		/* ====================  DISABLED      ======================================= */
		TrackerCenter(const TrackerCenter &);
		TrackerCenter &operator =(const TrackerCenter &);

}; /* -----  end of class TrackerCenter  ----- */

#endif
