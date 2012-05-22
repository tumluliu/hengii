/*
 * =====================================================================================
 *
 *       Filename:  torquejobfactory.cpp
 *
 *    Description:  factory to create torque job
 *
 *        Version:  1.0
 *        Created:  05/17/2012 08:43:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "torquejobfactory.h"
#include "torquejob.h"

TorqueJobFactory::~TorqueJobFactory() {}

std::unique_ptr<Player> TorqueJobFactory::Create(
		int64_t id, const Resources &rsrc, const std::string &cmdline) const {
	return std::unique_ptr<Player>(new TorqueJob(id, rsrc, cmdline));
}
