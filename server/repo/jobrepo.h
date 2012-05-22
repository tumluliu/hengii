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
#include <memory>

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
namespace hpgc { namespace higis { namespace interface { class JobFlow; }}}
class FlowRuntime;
class JobRuntime;

/*
 * =====================================================================================
 *        Class:  JobRepo
 *  Description:  He persists hundreds of job flows, all the year round
 * =====================================================================================
 */
class JobRepo
{
	public:
		/* ====================          LIFECYCLE ================================== */
		virtual ~JobRepo();
		/* ====================  jobflow LIFECYCLE ================================== */
		/* args: job flow runtime info; username */
		virtual int64_t RegisterJobFlow(
				const hpgc::higis::interface::JobFlow &,
				const std::string &) = 0;
		virtual std::unique_ptr<FlowRuntime> CreateFlowRuntime(int64_t) = 0;
		virtual void UpdateJobFlowRuntime(const FlowRuntime &) = 0;
		/* update only flow part of jobflow runtime, not including its jobs' */
		virtual void UpdateFlowPartRuntime(const FlowRuntime &) = 0;
		virtual void UpdateJobRuntime(const JobRuntime &) = 0;

	protected:
	private:
}; /* -----  end of class JobRepo  ----- */

#endif
