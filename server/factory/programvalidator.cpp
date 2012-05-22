/*
 * =====================================================================================
 *
 *       Filename:  programvalidator.cpp
 *
 *    Description:  program validator
 *
 *        Version:  1.0
 *        Created:  05/14/2012 05:52:34 PM
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

#include "programvalidator.h"
#include "appoption.h"
#include "metarepo.h"

ProgramValidator::ProgramValidator(const std::weak_ptr<JobValidator> next,
	   	const MetaRepo &metarepo)
	: JobValidator(next), metarepo_(metarepo) {
	}

bool ProgramValidator::LetPass(const hpgc::higis::interface::Job &job, 
		std::string &msg) const {
	if (!metarepo_.HasMeta(job.app_uri)) {
		msg = "start job error, program " + job.app_uri + " not exist";
		return false;
	}
	return true;
}
