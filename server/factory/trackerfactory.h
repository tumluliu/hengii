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

#ifndef  TRACKERFACTORY_H_
#define  TRACKERFACTORY_H_

#include <stdbool.h>
#include <string>
#include <vector>
#include <memory>

#include "trackerenv.h"
#include "jobtracker.h"
#include "player.h"
#include "flowruntime.h"
#include "validator.h"
#include "cmdlinebuilder.h"
#include "tracker.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
namespace hpgc { namespace higis { namespace interface { class JobFlow; }}}
class JobRepo;
class BatchJobFactory;
class MetaRepo;
class DataRepo;

/*
 * =====================================================================================
 *        Class:  TrackerFactory
 *  Description:  He makes tracker from flow, prepare all 
 *                environments needed(e.g. register in repo),
 *                He is strict so invalid flow can only end with a NULL pointer
 * =====================================================================================
 */
class TrackerFactory
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* constructor. args: flow; user name; DI fellows... */
		TrackerFactory(const hpgc::higis::interface::JobFlow &, const std::string &,
				JobRepo &, const BatchJobFactory &, const Validator &,
				MetaRepo &, DataRepo &);
		std::shared_ptr<Tracker> Create(int64_t) const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const std::string user_;
		const hpgc::higis::interface::JobFlow &flow_;
		JobRepo &repo_;
		const BatchJobFactory &batchfactory_;
		const Validator &guard_;
		MetaRepo &metarepo_;
		DataRepo &datarepo_;

		/* ====================  LIFECYCLE     ======================================= */
		/* args: failed message */
		void Fail(const std::string &, FlowRuntime &) const;

		/* ====================  STEPS         ======================================= */
		/* This section: return: 0 if success, -1 otherwise */
		/* args: (out)message */
		int Validate(std::string &) const;
		/* args: (out)message;flow runtime;flow id */
		int BuildJobTrackers(std::vector<std::shared_ptr<JobTracker> > &, 
				std::string&, FlowRuntime &, int64_t) const;
		/* args: job trackers list */
		int LinkJobTrackers(const std::vector<std::shared_ptr<JobTracker> > &) const;
		/* following 2: args:message; id */
		int LockData(std::string &, int64_t) const;

		/* ====================  DISABLED      ======================================= */
		TrackerFactory(const TrackerFactory &);
		TrackerFactory &operator =(const TrackerFactory &);
}; /* -----  end of class TrackerFactory  ----- */

#endif
