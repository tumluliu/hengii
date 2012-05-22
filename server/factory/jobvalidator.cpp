/*
 * =====================================================================================
 *
 *       Filename:  jobvalidator.cpp
 *
 *    Description:  job validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 03:14:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "jobvalidator.h"

JobValidator::~JobValidator() {}

JobValidator::JobValidator(const std::weak_ptr<JobValidator> next) : nextstage_(next) {
}

bool JobValidator::Chanllenge(const hpgc::higis::interface::Job &job,
		std::string &message) const {
	if (!LetPass(job, message)) {
		return false;
	}
	else if (auto gate = nextstage_.lock()) {
		return gate->LetPass(job, message);
	}
	else {
		return true;
	}
}
