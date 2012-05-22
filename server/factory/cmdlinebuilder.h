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
#include <string>

#include "appoption.h"

const std::string MPI_EXEC_CMD = "mpiexec ";

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
class MetaRepo;
namespace hpgc { namespace higis { namespace interface { class Context; }}}
namespace hpgc { namespace higis { namespace interface { class Job; }}}

/*
 * =====================================================================================
 *        Class:  CmdlineBuilder
 *  Description:  He build command line based on some meta data and a job.
 *                The class assumes the job is legal, so please validate the job
 *                before give it to him.
 * =====================================================================================
 */
class CmdlineBuilder
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CmdlineBuilder(const MetaRepo &);                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  TOOLS         ======================================= */
		/* args: 1:result, 2:job. return: fail/success signal */
		int Build(std::string &, const hpgc::higis::interface::Job &) const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const MetaRepo &repo_;
		/* ====================  HELPERS       ======================================= */
		/* args: 1:result;2:options map;3:app uri. return: signal*/
		int BuildAppOptions( 
				std::string &,
				const std::map<std::string, std::string> &options, 
				const std::string &) const; 
		int BuildLoader(
				std::string &, const hpgc::higis::interface::Context) const;
}; /* -----  end of class CmdlineBuilder  ----- */

#endif
