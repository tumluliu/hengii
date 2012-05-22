/*
 * =====================================================================================
 *
 *       Filename:  optionvalidator.h
 *
 *    Description:  validator for app options
 *
 *        Version:  1.0
 *        Created:  05/14/2012 03:31:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  OPTIONVALIDATIOR_H_
#define  OPTIONVALIDATIOR_H_

#include <stdbool.h>
#include <vector>

#include "jobvalidator.h"
#include "appoption.h"
#include "hpgcjob_types.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class MetaRepo;

/*
 * =====================================================================================
 *        Class:  OptionValidator
 *  Description:  He check all aspects of app options
 * =====================================================================================
 */
class OptionValidator : public JobValidator
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		OptionValidator(std::weak_ptr<JobValidator>, const MetaRepo &);

	protected:
		/* ====================  ACTIONS       ======================================= */
		virtual bool LetPass(const hpgc::higis::interface::Job &, std::string &) const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const MetaRepo &metarepo_;

		/* ====================  CHILD STAGES  ======================================= */
		bool LackEssential(std::string &, const hpgc::higis::interface::Job &, 
				const std::vector<AppOption> &meta) const;
		bool HasUnknown(std::string &, const hpgc::higis::interface::Job &,
				const std::vector<AppOption> &meta) const;
}; /* -----  end of class OptionValidator  ----- */

#endif
