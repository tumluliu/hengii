/*
 * =====================================================================================
 *
 *       Filename:  flowinfo.h
 *
 *    Description:  offers some info of flow  
 *
 *        Version:  1.0
 *        Created:  05/18/2012 04:03:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  FLOWINFO_H_
#define  FLOWINFO_H_

#include <vector>

#include "metarepo.h"

/*-----------------------------------------------------------------------------
 *  forward declarations to eliminate compiling dependencies
 *-----------------------------------------------------------------------------*/
namespace hpgc { namespace higis { namespace interface { class JobFlow; }}}
class DataRequest;

/*
 * =====================================================================================
 *        Class:  FlowInfo
 *  Description:  He contains a flow, and you can ask some questions about the job
 * =====================================================================================
 */
class FlowInfo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		FlowInfo(const hpgc::higis::interface::JobFlow &,
				const MetaRepo &); /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		/* args:(out)data requiring list */
		void get_datareqlist(std::vector<DataRequest> &);

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		const hpgc::higis::interface::JobFlow &data_;
		const MetaRepo &metarepo_;

}; /* -----  end of class FlowInfo  ----- */

#endif
