/*
 * =====================================================================================
 *
 *       Filename:  validator.cpp
 *
 *    Description:  jobflow validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 04:11:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sstream>

#include "validator.h"
#include "hpgcjob_types.h"

Validator::Validator(const JobValidator &sub) 
	: nextstage_(), substage_(sub) {}

Validator::Validator(const std::weak_ptr<Validator> next, const JobValidator &sub)
	: nextstage_(next), substage_(sub) {
	}

Validator::~Validator() {}

/* WARING: It is copy/paste programming, taking heavily dumplication with JobValidator.
 * However, job an job flow has no common ascentant, so one class is not enough. Surely
 * I can reduce the dump by delegates or macros or other methods, but will that help
 * a lot? I don't think so. * by YANG Anran @ 2012.5.14 */
bool Validator::Chanllenge(const hpgc::higis::interface::JobFlow &flow,
		std::string &message) const {
	if (!LetPass(flow, message)) {
		return false;
	}

	std::string innermsg;
	for (int i = 0; i < flow.job_count; i++) {
		if(!substage_.Chanllenge(flow.jobs[i], innermsg)) {
			std::stringstream ss;
			ss << "job " << flow.jobs[i].id << " : " << innermsg; 
			message = ss.str();
			return false;
		}
	}

	if (auto gate = nextstage_.lock()) {
		return gate->LetPass(flow, message);
	}

	return true;
}
