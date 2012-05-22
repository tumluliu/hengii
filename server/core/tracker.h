/*
 * =====================================================================================
 *
 *       Filename:  tracker.h
 *
 *    Description:  job flow tracker
 *
 *        Version:  1.0
 *        Created:  05/08/2012 08:33:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  TRACKER_H_
#define  TRACKER_H_

#include <vector>
#include <memory>

#include "recorder.h"
#include "player.h"
#include "flowruntime.h"
#include "jobtracker.h"


/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class JobRepo;
class TrackerEnv;
/*
 * =====================================================================================
 *        Class:  Tracker
 *  Description:  He starts a job flow, monitor it, and clean afterwards
 * =====================================================================================
 */
class Tracker : public Recorder, public Player
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* constructor. It only build tracker itsself, not its jobtrackers, for the 
		 * IOC convenience. Because if not like this, tracker but not its creator 
		 * MUST determine what batch job to use in jobtracker. He MUST also pull 
		 * JobFlow the thrift dependent and cmdline construction in (and further more, 
		 * its thousands of validations). It may be more intuitive that the tracker 
		 * builds all his players but... Oops, it's a disaster... */
		Tracker(std::unique_ptr<FlowRuntime>, JobRepo &, std::unique_ptr<TrackerEnv>);

		/* ====================  MUTATORS      ======================================= */
		void add_jobtracker(std::shared_ptr<JobTracker>);

		/* ====================  EVENTS        ======================================= */
		virtual void AllPlayersDone();

		/* ====================  ACTIONS       ======================================= */
		virtual void Play();

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::vector<std::shared_ptr<Player> > playlist_; 
		std::unique_ptr<FlowRuntime> state_;
		/* Now only use its auto destruction */
		std::unique_ptr<TrackerEnv> env_;
		JobRepo &repo_;
		bool isover_;

		/* ====================  DISABLED      ======================================= */
		Tracker(const Tracker &);
		Tracker &operator =(const Tracker &);

		/* ====================  ACTIONS       ======================================= */
		void LoadAllPlayers();

}; /* -----  end of class Tracker  ----- */


#endif
