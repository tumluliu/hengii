/*
 * =====================================================================================
 *
 *       Filename:  filemetarepo.cpp
 *
 *    Description:  repo of meta data, storing in .meta file
 *
 *        Version:  1.0
 *        Created:  05/17/2012 09:49:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "filemetarepo.h"
#include "config.h"
#include "utility.h"
#include "appoption.h"
#include "datarequest.h"
#include "log.h"

using std::string;

FileMetaRepo::~FileMetaRepo() {}

int FileMetaRepo::BuildMeta(std::vector<AppOption> &out_meta, 
		const string &appuri) const {
	string appMetaFile = APP_DIR + appuri + ".meta";

	std::ifstream metafile(appMetaFile.c_str(), std::ios::in);
	if (!metafile) { 
		return -1;
	}

	string record;

	string name;
	string isopt;
	string cmdswitch;
	string defaultvalue;
	/* extra, can be null */
	string datausage;
	string datatype;

	while (getline(metafile, record)) {
		std::stringstream ss(record);

		/* basic params */
		if (getline(ss, name, ',')
				&& getline(ss, isopt, ',') 
				&& getline(ss, cmdswitch, ',')
				&& getline(ss, defaultvalue, ',')) {
		}

		AppOption meta(name,
				isopt == "o" ? true : false,
				cmdswitch, defaultvalue);

		/* extra params, currently are data access type */
		/* WARNING: Here is a hard-coded field name. But It isn't much better using
		 * a const, because the implicit knowledge will be still scattered 
		 * by YANG Anran @ 2012.5.21 */
		if (getline(ss, datausage, ',')) {
			Log().Debug() << "data usage is: " << datausage;
			meta.add_extra("datausage", datausage);
			if (getline(ss, datatype, ',')) {
				meta.add_extra("datatype", datatype);
			}
		}

		out_meta.push_back(meta);
	}	

	metafile.close();

	return 0;
}

bool FileMetaRepo::HasMeta(const std::string &appuri) const {
	string appMetaFile = APP_DIR + appuri + ".meta";

	std::ifstream metafile(appMetaFile.c_str(), std::ios::in);
	if (metafile) {
		metafile.close();
		return true; 
	}
	else {
		return false;
	}
}

int FileMetaRepo::GetOptionMeta(AppOption &out_opt,
		const std::string &appuri, const std::string &name) const {
	std::vector<AppOption> meta;
	BuildMeta(meta, appuri);
	for (auto i = meta.begin(); i != meta.end(); i++) {
		if (i->get_name() == name) {
			out_opt = *i;
			return 0;
		}
	}
	return -1;
}
