/*
 * =====================================================================================
 *
 *       Filename:  appoption.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/2012 11:14:50 AM
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

#include "appoption.h"
#include "config.h"
#include "utility.h"

using std::vector;
using std::string;

AppOption::AppOption(const vector<string> &s) 
	: name_(s[0]), isoptional_(false), cmdswitch_(""), defaultvalue_("") {
		isoptional_ = s.size() > 1 && s[1] == "o" ? true : false;
		cmdswitch_ = s.size() > 2 ? s[2] : "" ;
		defaultvalue_ = s.size() > 3 ? s[3] : "";
	}

bool AppOption::IsUnknown(const vector<AppOption> &metas, 
		const string &offered) {
	for (size_t i = 0; i < metas.size(); i++) {
		if(metas[i].get_name() == offered) {
			return false;
		}
	}
	return true;
}


std::string AppOption::get_name() const {
	return name_;
}

bool AppOption::IsOptional() const {
	return isoptional_;
}

std::string AppOption::get_cmdswitch() const {
	return cmdswitch_;
}

std::string AppOption::get_defaultvalue() const {
	return defaultvalue_;
}

/* WARNING: This helper function is put here only TEMPORARILY.
 * by YANG Anran @ 2012.5.14 */
int AppOption::BuildMeta(
		std::vector<AppOption> &result, const string &appuri) {
	string appMetaFile = APP_DIR + appuri + ".meta";

	std::ifstream metafile(appMetaFile.c_str(), std::ios::in);
	if (!metafile) { 
		return -1;
	}

	string record;
	std::vector<string> pieces;
	while (getline(metafile, record)) {
		pieces = util::splitStringBySpace(record);
		AppOption appOptionMeta(pieces);
		result.push_back(appOptionMeta);
	}	

	metafile.close();

	return 0;
}


bool AppOption::HasMeta(const std::string &appuri) {
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
