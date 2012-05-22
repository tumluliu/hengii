/*
 * =====================================================================================
 *
 *       Filename:  filemetarepo.h
 *
 *    Description:  repo of meta data, storing in .meta file
 *
 *        Version:  1.0
 *        Created:  05/17/2012 09:46:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  FILEMETAREPO_H_
#define  FILEMETAREPO_H_

#include <string>

#include "metarepo.h"

/*
 * =====================================================================================
 *        Class:  FileMetaRepo
 *  Description:  He stores meta info of apps in .meta files
 * =====================================================================================
 */
class FileMetaRepo : public MetaRepo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~FileMetaRepo();                             /* constructor */

		/* ====================  TOOLS         ======================================= */
		/* args: 1:result;2:app uri. return:signal */
		virtual int BuildMeta(std::vector<AppOption> &, const std::string &) const;
		/* args: app uri */
		virtual bool HasMeta(const std::string &) const;
		/* args: (out)option;app uri;option name; return: sig, 0 if found, -1 otherwise */
		virtual int GetOptionMeta(AppOption &,
			   const std::string &,	const std::string &) const;
		
	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class FileMetaRepo  ----- */

#endif
