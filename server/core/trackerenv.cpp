/*
 * =====================================================================================
 *
 *       Filename:  trackerenv.cpp
 *
 *    Description:  tracker env, contains resources cleanup functionality
 *
 *        Version:  1.0
 *        Created:  05/21/2012 05:45:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "trackerenv.h"
#include "datarepo.h"

TrackerEnv::TrackerEnv(DataRepo &datarepo, 
		int64_t id) : datarepo_(datarepo), trackerid_(id) {
}

TrackerEnv::~TrackerEnv() {
	datarepo_.UnLock(trackerid_);
}
