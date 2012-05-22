/*
 * =====================================================================================
 *
 *       Filename:  validator.h
 *
 *    Description:  flow validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 04:10:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  VALIDATOR_H_
#define  VALIDATOR_H_

#include <memory>

#include "jobvalidator.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
namespace hpgc { namespace higis { namespace interface { class JobFlow; }}}

/*
 * =====================================================================================
 *        Class:  Validator
 *  Description:  He checks a flow and his jobs, in a game stage manner, pls refer to
 *                job validator to known more
 * =====================================================================================
 */
class Validator
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Validator(const JobValidator &); /* constructor */
		Validator(const std::weak_ptr<Validator>, const JobValidator &); 
		virtual ~Validator();

		/* ====================  ACTIONS       ======================================= */
		bool Chanllenge(const hpgc::higis::interface::JobFlow &, std::string &) const;

	protected:
		/* ====================  ACTIONS       ======================================= */
		/* WARNING: the validator should be a abstract class, but now no flow checking
		 * is taken, so it is simply defined as a concrete class. by YANG Anran @
		 * 2012.5.14 */
		virtual bool LetPass(const hpgc::higis::interface::JobFlow &,
				std::string &) const { return true; };

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const std::weak_ptr<Validator> nextstage_;
		const JobValidator &substage_;
		/* ====================  DISABLED      ======================================= */
		Validator &operator =(const Validator &);
		Validator(const Validator &);
}; /* -----  end of class Validator  ----- */

#endif
