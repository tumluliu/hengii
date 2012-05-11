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
#include "jobrepoentry.h"
#include "log.h"

Tracker::Tracker(FlowRuntime *state) 
	: Player(state->get_id()), state_(state), isover_(false) {
}

void Tracker::AllPlayersDone() {
	Log().Debug() << "All jobs of " << get_id() << " are done.";
	state_->set_status(Status::FINISHED);
	JobRepoEntry::Open()->UpdateFlowPartRuntime(state_);
	isover_ = true;
}

void Tracker::Play() {
	Log().Debug() << "[MsgFlow] Start jobs";
	LoadAllPlayers();
	while(!isover_) {
		Log().Debug() << get_id() << " is playing...";
		sleep(1);
	}
	Log().Debug() << "[MsgFlow] Report flow " << get_id()
		<< " completion to " << get_recorders_num() << " recorders";
	/* WARNING: not very elegant, but the only way to ensure that
	 * no action is take after reporting completion(which leads to 
	 * destruction) */
	get_recorder(0)->OnePlayerDone(get_id());
}
