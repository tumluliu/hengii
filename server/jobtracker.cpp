/*
 * =====================================================================================
 *
 *       Filename:  jobtracker.cpp
 *
 *    Description:  monitor a job, persist its status, 
 *                  notify his followers and tracker
 *
 *        Version:  1.0
 *        Created:  05/08/2012 09:51:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "jobtracker.h"
#include "log.h"
#include "jobrepoentry.h"

JobTracker::JobTracker(Player *job, JobRuntime *state) 
	: ChainPlayer(state->get_id()), isover_(false), job_(job), state_(state) {
	}

void JobTracker::PlayMySong() {
	ClearPlaylist();
	ListenPlayer(job_);
	state_->set_status(JobStatus::QUEUING);
	JobRepoEntry::Open()->UpdateJobRuntime(state_);
	Log().Debug() << "[MsgFlow] Start torque job, I'm job " << get_id();
	LoadAllPlayers();
	while(!isover_) {}
}

void JobTracker::AllPlayersDone() {
	if (!IsMyTurn()) {
		Log().Debug() << "[MsgFlow] Parents jobs complete, I'm job " << get_id();
		ChainPlayer::AllPlayersDone();
	}
	else {
		Log().Debug() << "[MsgFlow] Torque job complete, I'm job " << get_id();
		Log().Info() << "job complete, result is: " << job_->get_output();
		state_->set_message(job_->get_output());
		state_->set_status(JobStatus::FINISHED);
		JobRepoEntry::Open()->UpdateJobRuntime(state_);
		isover_ = true;
	}
}
