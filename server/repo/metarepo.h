/*
 * =====================================================================================
 *
 *       Filename:  metarepo.h
 *
 *    Description:  repo of meta data
 *
 *        Version:  1.0
 *        Created:  05/17/2012 09:28:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  METAREPO_H_
#define  METAREPO_H_

#include <string>
#include <vector>

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class AppOption;

/*
 * =====================================================================================
 *        Class:  MetaRepo
 *  Description:  He stores the meta data of apps
 * =====================================================================================
 */
class MetaRepo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~MetaRepo();                             /* constructor */

		/* ====================  TOOLS         ======================================= */
		/* args: 1:result;2:app uri. return:signal */
		virtual int BuildMeta(std::vector<AppOption> &, const std::string &) const = 0;
		/* args: app uri */
		virtual bool HasMeta(const std::string &) const = 0;
		/* args: (out)option;app uri;option name; return: sig, 0 if found, -1 otherwise */
		virtual int GetOptionMeta(AppOption &,
			   const std::string &, const std::string &) const = 0;

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class MetaRepo  ----- */

#endif
