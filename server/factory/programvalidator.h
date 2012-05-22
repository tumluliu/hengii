/*
 * =====================================================================================
 *
 *       Filename:  programvalidator.h
 *
 *    Description:  check program and metadata existence, etc.
 *
 *        Version:  1.0
 *        Created:  05/14/2012 05:48:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PROGRAMVALIDATOR_H_
#define  PROGRAMVALIDATOR_H_

#include "jobvalidator.h"
#include "hpgcjob_types.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class MetaRepo;

/*
 * =====================================================================================
 *        Class:  ProgramValidator
 *  Description:  He checks if a program and his metafile exists
 * =====================================================================================
 */
class ProgramValidator : public JobValidator
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		ProgramValidator(const std::weak_ptr<JobValidator> , const MetaRepo &);
	protected:
		/* ====================  ACTIONS       ======================================= */
		virtual bool LetPass(const hpgc::higis::interface::Job &, std::string &) const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const MetaRepo &metarepo_;
}; /* -----  end of class ProgramValidator  ----- */

#endif
