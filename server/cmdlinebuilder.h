/*
 * =====================================================================================
 *
 *       Filename:  cmdlinebuilder.h
 *
 *    Description:  build command line for job
 *
 *        Version:  1.0
 *        Created:  05/09/2012 11:00:43 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  CMDLINEBUILDER_H_
#define  CMDLINEBUILDER_H_

#include <map>

#include "appoption.h"
#include "hpgcjob_types.h"

const std::string MPI_EXEC_CMD = "mpiexec ";

/*
 * =====================================================================================
 *        Class:  CmdlineBuilder
 *  Description:  He build command line based on some meta data and a job
 * =====================================================================================
 */
class CmdlineBuilder
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CmdlineBuilder();                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  TOOLS         ======================================= */
		/* args: 1:result, 2:job. return: fail/success signal */
		int Build(std::string &, const hpgc::higis::interface::Job &);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		/* ====================  HELPERS       ======================================= */
		/* args: 1:result;2:options map;3:app uri. return: signal*/
		int BuildAppOptions( 
				std::string &,
				const std::map<std::string, std::string> &options, 
				const std::string &); 
		/* args: 1:result;2:app uri. return:signal */
		int BuildAppOptionsMeta(
				std::vector<AppOption> &, const std::string &);
		int BuildLoader(
				std::string &, const hpgc::higis::interface::Context);
}; /* -----  end of class CmdlineBuilder  ----- */

#endif
