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

#include "chainplayer.h"
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
class JobTracker : public ChainPlayer
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* args: 1:inner batch job;2:runtime;3:repo */
		JobTracker(std::unique_ptr<Player>, std::shared_ptr<JobRuntime>, JobRepo &);

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

	protected:
		/* ====================  ACTIONS       ======================================= */
		void PlayMySong();

		/* ====================  EVENTS        ======================================= */
		virtual void AllPlayersDone();

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::unique_ptr<Player> job_;
		std::shared_ptr<JobRuntime> state_;
		JobRepo &repo_;
		bool isover_;

		/* ====================  LIFECYCLE     ======================================= */
		JobTracker(const JobTracker &);
		JobTracker &operator =(const JobTracker &);
}; /* -----  end of class JobTracker  ----- */


#endif
