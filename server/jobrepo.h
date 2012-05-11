/*
 * =====================================================================================
 *
 *       Filename:  jobrepo.h
 *
 *    Description:  Abstraction of a job repo
 *
 *        Version:  1.0
 *        Created:  05/03/2012 08:09:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  JOBREPO_H_
#define  JOBREPO_H_

#include <stdint.h>

#include "hpgcjob_types.h"
#include "flowruntime.h"

/* forward declaration, because he must be dbjobrepo's friend */
class JobRepoEntry;

/*
 * =====================================================================================
 *        Class:  JobRepo
 *  Description:  He persists hundreds of job flows, all the year round
 * =====================================================================================
 */
class JobRepo
{
	public:
		/* ====================  jobflow LIFECYCLE ================================== */
		/* args: job flow runtime info; username */
		virtual int64_t RegisterJobFlow(
				const hpgc::higis::interface::JobFlow &,
				const std::string &) = 0;
		virtual FlowRuntime *BorrowJobFlowRuntime(int64_t) = 0;
		virtual void ReturnJobFlowRuntime(FlowRuntime*) = 0;
		virtual void UpdateJobFlowRuntime(const FlowRuntime *) = 0;
		/* update only flow part of jobflow runtime, not including its jobs' */
		virtual void UpdateFlowPartRuntime(const FlowRuntime *) = 0;
		virtual void UpdateJobRuntime(const JobRuntime *) = 0;

		/* ====================  friend, the only one to create self ================= */
		friend class JobRepoEntry;

	protected:
		virtual ~JobRepo();
	private:
}; /* -----  end of class JobRepo  ----- */

#endif
