/*
 * =====================================================================================
 *
 *       Filename:  batchjobfactory.h
 *
 *    Description:  factory to create batch job
 *
 *        Version:  1.0
 *        Created:  05/17/2012 08:35:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  BATCHJOBFACTORY_H_
#define  BATCHJOBFACTORY_H_

#include <memory>
#include <string>

#include "resources.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class Player;

/*
 * =====================================================================================
 *        Class:  BatchJobFactory
 *  Description:  He produces batch job
 * =====================================================================================
 */
class BatchJobFactory
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~BatchJobFactory();

		/* ====================  FACTORY METHOD  ===================================== */
		/* In the future may return a class named batch job, 
		 * but now a player is enough.
		 * args: id, resources requested, cmdline */
		virtual std::unique_ptr<Player> Create(
				int64_t, const Resources &, const std::string &) const = 0;
}; /* -----  end of class BatchJobFactory  ----- */

#endif
