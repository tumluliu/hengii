/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.h
 *
 *    Description:  monitor a job, persist its status, 
 *                  notify his followers and tracker
 *
 *        Version:  1.0
 *        Created:  05/07/2012 08:49:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  JOBTRACKER_H_
#define  JOBTRACKER_H_

#include <stdbool.h>

#include "player.h"
#include "recorder.h"
#include "jobruntime.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class JobRepo;
/*
 * =====================================================================================
 *        Class:  JobTracker
 *  Description:  He monitors the real job, everytime persists status change,
 *                notify his followers and flow tracker
 * =====================================================================================
 */
class JobTracker : public Player, public Recorder
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* args: 1:inner batch job;2:runtime;3:repo */
		JobTracker(std::unique_ptr<Player>, std::shared_ptr<JobRuntime>, JobRepo &);
		virtual ~JobTracker();

		/* ====================  ACTIONS       ======================================= */
		virtual void Stop();

	protected:
		/* ====================  ACTIONS       ======================================= */
		void Play();

		/* ====================  EVENTS        ======================================= */
		virtual void AllPlayersDone();
		virtual void AllPlayersCanceled();

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::unique_ptr<Player> job_;
		std::shared_ptr<JobRuntime> state_;
		JobRepo &repo_;

		/* ====================  SIGNALS       ======================================= */
		bool myturn_;
		bool cancel_;

		/* ====================  DISABLED      ======================================= */
		JobTracker(const JobTracker &);
		JobTracker &operator =(const JobTracker &);

		/* ====================  HELPERS       ======================================= */
		void UpdateStatusAndNotify(JobStatus::type);
}; /* -----  end of class JobTracker  ----- */


#endif
