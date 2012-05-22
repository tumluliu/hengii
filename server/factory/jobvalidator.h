/*
 * =====================================================================================
 *
 *       Filename:  jobvalidator.h
 *
 *    Description:  job validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 03:05:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  JOBVALIDATOR_H_
#define  JOBVALIDATOR_H_

#include <stdbool.h>
#include <string>
#include <memory>

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
namespace hpgc { namespace higis { namespace interface { class Job; }}}

/*
 * =====================================================================================
 *        Class:  JobValidator
 *  Description:  The interface of job validator. He acts like a game stage, pass it
 *                will get you to the next stage, i.e. to the next validator. If there
 *                is no one, i.e. itself is the final boss, then the game 
 *                is "stage cleared", the job is legal.
 * =====================================================================================
 */
class JobValidator
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		JobValidator(const std::weak_ptr<JobValidator>);        /* constructor */
		virtual ~JobValidator();

		/* ====================  ACTIONS       ======================================= */
		bool Chanllenge(const hpgc::higis::interface::Job &, std::string &) const;

	protected:
		/* ====================  ACTIONS       ======================================= */
		virtual bool LetPass(const hpgc::higis::interface::Job &,
			   	std::string &) const = 0;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const std::weak_ptr<JobValidator> nextstage_;
		/* ====================  DISABLED      ======================================= */
		JobValidator(const JobValidator &);
		JobValidator &operator =(const JobValidator &);
}; /* -----  end of class JobValidator  ----- */

#endif

