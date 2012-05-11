/*
 * =====================================================================================
 *
 *       Filename:  jobrepoentry.h
 *
 *    Description:  the only entry of job repo
 *
 *        Version:  1.0
 *        Created:  05/03/2012 09:08:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "jobrepo.h"

/*
 * =====================================================================================
 *        Class:  JobRepoEntry
 *  Description:  The only entry to get to a job repo
 * =====================================================================================
 */
class JobRepoEntry
{
	public:
		/* ====================  LIFECYCLE(As singleton)  ============================ */
		static JobRepo *Open();
		static void Close();
	protected:
	private:
		/* ====================  INSTANCE(singleton)      ============================ */
		static JobRepo *m_instance;

}; /* -----  end of class JobRepoEntry  ----- */

