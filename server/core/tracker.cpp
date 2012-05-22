/*
 * =====================================================================================
 *
 *       Filename:  tracker.cpp
 *
 *    Description:  job flow tracker
 *
 *        Version:  1.0
 *        Created:  05/08/2012 09:09:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "tracker.h"
#include "jobrepo.h"
#include "trackerenv.h"
#include "log.h"

Tracker::Tracker(std::unique_ptr<FlowRuntime> state, JobRepo &repo, 
		std::unique_ptr<TrackerEnv> env) 
	: Player(state->get_id()), state_(std::move(state)), env_(std::move(env)), 
	repo_(repo), isover_(false) {
	}

void Tracker::AllPlayersDone() {
	Log().Debug() << "All jobs of " << get_id() << " are done.";
	state_->set_status(Status::FINISHED);
	repo_.UpdateFlowPartRuntime(*state_);
	isover_ = true;
}

void Tracker::Play() {
	Log().Debug() << "[MsgFlow] Start jobs";
	LoadAllPlayers();
	while(!isover_) {
		//Log().Debug() << get_id() << " is playing...";
	}
	Log().Debug() << "[MsgFlow] Report flow " << get_id()
		<< " completion to " << get_recorders_num() << " recorders";
	/* WARNING: not very elegant, but the only way to ensure that
	 * no action is take after reporting completion(which leads to 
	 * destruction) (This is not the case even when using for_each) */
	if (auto boss = get_recorder(0).lock()) {
		boss->OnePlayerDone(get_id());
	}
}

void Tracker::LoadAllPlayers() {
	size_t i;

	for (i = 0; i < playlist_.size(); i++) {
		playlist_[i]->GoPlay();
	}
}

void Tracker::add_jobtracker(std::shared_ptr<JobTracker> child) {
	playlist_.push_back(child);
	ListenPlayer(*child);
}
