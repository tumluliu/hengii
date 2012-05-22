/*
 * =====================================================================================
 *
 *       Filename:  torquejobfactory.h
 *
 *    Description:  factory to create torque job
 *
 *        Version:  1.0
 *        Created:  05/17/2012 08:41:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  TORQUEJOBFACTORY_H_
#define  TORQUEJOBFACTORY_H_

#include "batchjobfactory.h"

/*
 * =====================================================================================
 *        Class:  TorqueJobFactory
 *  Description:  He produces torque job
 * =====================================================================================
 */
class TorqueJobFactory : public BatchJobFactory
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~TorqueJobFactory();                             /* constructor */

		/* ====================  FACTORY METHOD  ===================================== */
		/* In the future may return a class named batch job, 
		 * but now a player is enough.
		 * args: id, resources requested, cmdline */
		virtual std::unique_ptr<Player> Create(
				int64_t, const Resources &, const std::string &) const;
}; /* -----  end of class TorqueJobFactory  ----- */

#endif
