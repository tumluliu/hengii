/*
 * =====================================================================================
 *
 *       Filename:  flowinfo.cpp
 *
 *    Description:  offers some info of flow
 *
 *        Version:  1.0
 *        Created:  05/20/2012 10:34:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "flowinfo.h"
#include "datarequest.h"
#include "hpgcjob_types.h"
#include "appoption.h"
#include "log.h"
#include "utility.h"

FlowInfo::FlowInfo(const hpgc::higis::interface::JobFlow &flow,
		const MetaRepo &metarepo) : data_(flow), metarepo_(metarepo) {
}

void FlowInfo::get_datareqlist(std::vector<DataRequest> &out_datareqs) {
	/* WARNING: ugly codes here. needs detailed design of meta file and 
	 * unify of data id. by YANG Anran @ 2012.5.21 */
	for (auto i = data_.jobs.begin(); i != data_.jobs.end(); i++) {
		for (auto j = i->app_options.begin(); j != i->app_options.end(); j++) {
			AppOption opt;
			metarepo_.GetOptionMeta(opt, i->app_uri, j->first);
			std::string usagestr;
			if (opt.get_extra(usagestr, "datausage") == 0) {
				Access::type usage;
				if (usagestr == "c") {
					usage = Access::CREATE;
				}
				else if (usagestr == "u") {
					usage = Access::UPDATE;
				}
				else if (usagestr == "r") {
					usage = Access::READ;
				}
				else {
					continue;
				}
				std::string datatype;
				opt.get_extra(datatype, "datatype");

				out_datareqs.push_back(DataRequest(j->second, usage, 
							static_cast<DataType::type>(util::stoi(datatype))));
			}
		}
	}
}
