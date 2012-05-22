/*
 * =====================================================================================
 *
 *       Filename:  datarepo.h
 *
 *    Description:  the entry to operate on data privillieges
 *
 *        Version:  1.0
 *        Created:  05/18/2012 03:00:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  DATAREPO_H_
#define  DATAREPO_H_

#include <string>
#include <vector>

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class DataRequest;

/*
 * =====================================================================================
 *        Class:  DataRepo
 *  Description:  He offers some mechanics to ensure the data to be correctly accessed
 * =====================================================================================
 */
class DataRepo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~DataRepo();
		
		/* ====================  TOOLS         ======================================= */
		/* args: (out)fail message;the id of requesting flow instance;requests */
		virtual int Lock(std::string &, int64_t, const std::vector<DataRequest> &) = 0;
		/* args: the id of requesting flow instance */
		virtual void UnLock(int64_t) = 0;

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
}; /* -----  end of class DataRepo  ----- */

#endif
