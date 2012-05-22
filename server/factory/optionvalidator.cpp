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
#include "metarepo.h"
#include "log.h"

using hpgc::higis::interface::Job;

OptionValidator::OptionValidator(const std::weak_ptr<JobValidator> next,
	   	const MetaRepo &metarepo) 
	: JobValidator(next), metarepo_(metarepo) {
	}

bool OptionValidator::LetPass(const Job &job, std::string &message) const {
	std::vector<AppOption> meta;
	metarepo_.BuildMeta(meta, job.app_uri);
	return (!LackEssential(message, job, meta) 
			&& !HasUnknown(message, job, meta));
}

bool OptionValidator::LackEssential(std::string &message,
		const Job &job, const std::vector<AppOption> &meta) const {
	for (size_t i = 0; i < meta.size(); i++) {
		if (meta[i].IsOptional() == false 
				&& job.app_options.count(meta[i].get_name()) == 0) {
			message = "start job error, lack option " + meta[i].get_name();
			return true;
		}
	}
	return false;
}

bool OptionValidator::HasUnknown(std::string &out_msg,
		const Job &job, const std::vector<AppOption> &meta) const {
	for (std::map<std::string, std::string>::const_iterator it = job.app_options.begin();
			it != job.app_options.end(); it++) {
		bool unknown = true;

		for (size_t i = 0; i < meta.size(); i++) {
			if(meta[i].get_name() == it->first) {
				unknown = false;
				Log().Debug() << "validating...";
				break;
			}
		}

		if (unknown) {
			out_msg = "start job error, option " + (*it).first + " unknown";
			return true;
		}
	}
	return false;
}
