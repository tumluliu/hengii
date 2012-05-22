/*
 * =====================================================================================
 *
 *       Filename:  trackercenter.cpp
 *
 *    Description:  the manager of all trackers
 *
 *        Version:  1.0
 *        Created:  05/17/2012 08:21:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "trackercenter.h"
#include "log.h"
#include "tracker.h"

TrackerCenter::TrackerCenter(JobRepo &repo) : repo_(repo),
	trackers_(), lock_() {
		pthread_mutex_init(&lock_, NULL);
	}

TrackerCenter::~TrackerCenter() {
	pthread_mutex_destroy(&lock_);
}

void TrackerCenter::OnePlayerDone(int64_t id) {
	Log().Debug() << "[MsgFlow] Flow " << id << " complete";

	Log().Debug() << "[MsgFlow] Flow " << id << " die";

	pthread_mutex_lock(&lock_);
	trackers_.erase(id);
	pthread_mutex_unlock(&lock_);
}

void TrackerCenter::add_tracker(std::shared_ptr<Tracker> car) {
	pthread_mutex_lock(&lock_);

	car->add_recorder(shared_from_this());

	Log().Debug() << "[MsgFlow] Start flow " << car->get_id();
	car->GoPlay();

	trackers_.insert(std::make_pair(car->get_id(), car));

	pthread_mutex_unlock(&lock_);
}
