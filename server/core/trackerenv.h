/*
 * =====================================================================================
 *
 *       Filename:  trackerenv.h
 *
 *    Description:  tracker env, contains resources cleanup functionality
 *
 *        Version:  1.0
 *        Created:  05/21/2012 04:38:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  TRACKERENV_H_
#define  TRACKERENV_H_

/* DESIGN DECISION: 
 * This class abstract the object runtime env, which will do some cleanup after the 
 * object die. 
 * I previously make this a wrapper, and do a lot analysis. But when I reverse the relationship 
 * and change it to the metaphor of env, everything seems finally get its place. In fact, 
 * it is a nice journey of this refactoring because the code is less and I remove 
 * several warnings.
 *
 * I didn't delete the part of previous analysis below because it is still inspiring, 
 * especially for item 1. As for item 2, it is nearly deprecated but the conclusion still
 * helps, except that it should be now state as: 
 *
 * this design consists of a TYPICAL factory and a less typical but 
 * rather CLEAR runtime env(the env clean himself just like system clean a object's living
 * env. e.g. some index or statistic data)
 *
 * by YANG Anran @ 2012.5.22
 *
 * This class exists due to the needs for some cleanups related to 
 * runtime env. Till now, it is only for lock/unlock data that the tracker consumes. 
 * There are several methods to do the job. Except for this, the unlock logic can be 
 * simply put into the tracker's destructor. But on that senario, the tracker's interface is
 * polluted and may explode when extra cleanup needs occur. 
 *
 * This class wraps the tracker, which decomples the run&trace logic from cleanup, and the 
 * cleanup can be easily acheived by a smart pointer to the wrapper class. 
 *
 * some potential worrying and revisons:
 * 1. The resource allocate/cleanup are not balanced. Factory consumes resource and 
 * this class free them
 * revision: Resources does NOT have to be balanced in this manner. With an
 * unambigious ownership shipping(factory creates a product), the cleanup 
 * responsibility now falls on the receiver class.
 * In fact, the balanced manner may cause some unefficiency due to the imbalance lying in
 * the knowledge requirements of create/destroy: in most cases, destroy is far easier.
 * (the same in real life, isn't it?) So, it may not be a wise choice to always let the
 * creator waiting for long long time only to do little work comparing to the creating
 * task. Rather, the creator just transfer ownership and the receiver will do all the
 * other work. Further more, it seems factory returning smart pointer, or even GC in
 * smarter languages works in this manner.
 * 2. I feel bad as if the borrow/return factory is back, now even with a class more
 * it seems the borrow/return factory is back. 
 * revision: I cannot persuade myself fully, but to get back borrow/return factory is
 * less clear than this design. Reasons: 1) the borrow/return factory must be held 
 * too to wait for the returning. Then if it is used as this class, i.e. as a wrapper 
 * and tracker center simply keeps its smart pointer, the complex building logic seems 
 * not proper. 2) this design consists of a TYPICAL factory and a less typical but 
 * rather CLEAR resource ptr(It is like the smart pointer not only manages memory but
 * also other resources). 
 * by YANG Anran @ 2012.5.21 */

#include <memory>

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class DataRepo;

/*
 * =====================================================================================
 *        Class:  TrackerEnv
 *  Description:  He wrap tracker, do some cleanup when dies
 * =====================================================================================
 */
class TrackerEnv
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		TrackerEnv(DataRepo &, int64_t);/* constructor for empty ptr */
		~TrackerEnv();

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		DataRepo &datarepo_;
		int64_t trackerid_;
		
		/* ====================  DISABLED      ======================================= */
		/* These MUST be disabled because when it is destructed it will free 
		 * resources of its internal tracker, then the copy will leads to too early free
		 * or even double free! */
		TrackerEnv(const TrackerEnv &);
		TrackerEnv &operator= (const TrackerEnv &); 
}; /* -----  end of class TrackerEnv  ----- */

#endif
