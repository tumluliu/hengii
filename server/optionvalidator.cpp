/*
 * =====================================================================================
 *
 *       Filename:  optionvalidator.cpp
 *
 *    Description:  app options validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 03:36:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "optionvalidator.h"

using hpgc::higis::interface::Job;

bool OptionValidator::LetPass(const Job &job, std::string &message) const {
	std::vector<AppOption> meta;
	AppOption::BuildMeta(meta, job.app_uri);
	return (LackEssential(message, job, meta) 
			&& HasUnknown(message, job, meta));
}

bool OptionValidator::LackEssential(std::string &message,
		const Job &job, const std::vector<AppOption> &meta) const {
	for (size_t i = 0; i < meta.size(); i++) {
		if (meta[i].IsOptional() == false 
				&& job.app_options.count(meta[i].get_name()) == 0) {
			message = "start job error, lack option " + meta[i].get_name();
			return false;
		}
	}
	return true;
}

bool OptionValidator::HasUnknown(std::string &message,
		const Job &job, const std::vector<AppOption> &meta) const {
	for (std::map<std::string, std::string>::const_iterator it = job.app_options.begin();
			it != job.app_options.end(); it++) {
		if (AppOption::IsUnknown(meta, (*it).first)) {
			message = "start job error, option " + (*it).first + " unknown";
			return false;
		}
	}
	return true;
}
