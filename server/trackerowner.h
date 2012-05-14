/*
 * =====================================================================================
 *
 *       Filename:  trackerfactory.h
 *
 *    Description:  build tracker from job flow
 *
 *        Version:  1.0
 *        Created:  05/08/2012 08:32:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  TRACKEROWNER_H_
#define  TRACKEROWNER_H_

#include <stdbool.h>
#include <string>
#include <vector>

#include "tracker.h"
#include "jobtracker.h"
#include "player.h"
#include "flowruntime.h"
#include "hpgcjob_types.h"
#include "validator.h"

/*
 * =====================================================================================
 *        Class:  TrackerOwner
 *  Description:  He makes tracker from flow, prepare all 
 *                environments needed(e.g. register in repo),
 *                you can borrow only one from him, and remember to return
 *                it. Otherwise you will get punished by "memory leak".
 *                And after that you cannot still use the tracker, or you will
 *                get punished by "wild pointer".
 *                He is strict so invalid flow can only end with a NULL pointer
 *                WARNING: The design here is intuitive but may not fit the well-known
 *                design-patterns. For some part it is a factory, but offers destructor.
 *                And to acheive this, the class becomes a less powerful factory because
 *                it can offer only one product at one time.
 * =====================================================================================
 */
class TrackerOwner
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* constructor. args: flow; user name */
		TrackerOwner(const hpgc::higis::interface::JobFlow &, const std::string &);
		/* args: id. WARNING: The id returned is a workaround. When flow got some 
		 * error, no tracker will be returned, while flow runtime has been 
		 * generated and client still needs to know the id. It probably indicates design 
		 * fault due to concept misused. by YANG Anran @ 2012.5.10 */
		Tracker *Borrow(int64_t &);
		void Return(Tracker *);

		/* ====================  CONFIGURATION ======================================= */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string user_;
		hpgc::higis::interface::JobFlow flow_;
		Tracker *product_;
		std::vector<Player *> jobtrackers_;
		std::vector<Player *> batchjobs_;
		FlowRuntime *state_;
		bool isborrowed_;

		/* ====================  LIFECYCLE     ======================================= */
		void CleanUp();
		/* args: failed job id; message */
		void Fail(int, const std::string &);
		/* args: failed message */
		void Fail(const std::string &);

		/* ====================  STEPS         ======================================= */
		int Validate();
		/* args: flow id */
		int BuildJobTrackers(int64_t);
		int LinkJobTrackers();

		/* ====================  DISABLED      ======================================= */
		TrackerOwner(const TrackerOwner &);
		TrackerOwner &operator =(const TrackerOwner &);
}; /* -----  end of class TrackerOwner  ----- */

#endif
