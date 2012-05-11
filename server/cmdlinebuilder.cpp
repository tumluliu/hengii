/*
 * =====================================================================================
 *
 *       Filename:  cmdlinebuilder.cpp
 *
 *    Description:  build command line for job
 *
 *        Version:  1.0
 *        Created:  05/09/2012 11:20:28 AM
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

#include "cmdlinebuilder.h"
#include "utility.h"
#include "config.h"

using std::string;
using hpgc::higis::interface::Job;
using hpgc::higis::interface::Context;
using hpgc::higis::interface::ParallelEnv;

CmdlineBuilder::CmdlineBuilder() {
}

int CmdlineBuilder::Build(string &result, const Job &job) {
	result = "";
	string cache = "";

	if(BuildLoader(cache, job.runtime_context) != 0) { 
		result = cache;
		return -1;
	}
	result += cache;

	result += APP_DIR + job.app_uri + " ";

	if (BuildAppOptions(cache, job.app_options, job.app_uri) != 0) {
		result = cache;
		return -1;
	}
	result += cache;

	return 0;
}

int CmdlineBuilder::BuildAppOptions(
		string & result, const std::map<string, string> &options, 
		const std::string &appuri) {
	result = "";
	std::vector<AppOption> appOptionMetas;

	if (BuildAppOptionsMeta(appOptionMetas, appuri) != 0) {
		result = "start job error, program " + appuri + " not exist";
		return -1;
	}

	for (size_t i = 0; i < appOptionMetas.size(); i++) {
		bool lackEssential = true;
		for (std::map<string, string>::const_iterator it = options.begin();
				it != options.end(); it++) {
			if(appOptionMetas[i].get_name() == (*it).first) {
				result = result + 
					appOptionMetas[i].get_cmdswitch() + " " + (*it).second + " ";
				lackEssential = false;
			}
		}
		if (appOptionMetas[i].IsOptional() == false && lackEssential == true) {
			result = "start job error, lack option " + appOptionMetas[i].get_name();
			return -1;
		}
	}

	for (std::map<string, string>::const_iterator it = options.begin(); 
			it != options.end(); it++) {
		if (AppOption::IsUnknown(appOptionMetas, (*it).first)) {
			result = "start job error, option " + (*it).first + " unknown";
			return -1;
		}
	}

	return 0;
}

int CmdlineBuilder::BuildAppOptionsMeta(
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

	return 0;
}

int CmdlineBuilder::BuildLoader(string &result, Context env) {
	if (env.parallel_env == ParallelEnv::MPI) {
		result = MPI_EXEC_CMD;
		result += "-n " + env.options["process_count"] + " ";

		string mpiconf = ""; 
		if (util::readFile(MPI_CONF_PATH, mpiconf) != 0) {
			result = "unable to open mpi config file " + MPI_CONF_PATH;
			return -1;
		}
		result += mpiconf + " ";
	} /* other envs here */
	else { 
		result = ""; 
	}
	return 0;
}
