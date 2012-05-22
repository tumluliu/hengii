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
#include <memory>

#include "jobtracker.h"
#include "log.h"
#include "jobrepo.h"

JobTracker::JobTracker(std::unique_ptr<Player> job,
	   	std::shared_ptr<JobRuntime> state, JobRepo &repo) 
	: ChainPlayer(state->get_id()), job_(std::move(job)), state_(state), 
	repo_(repo), isover_(false) {
	}

void JobTracker::PlayMySong() {
	ClearPlaylist();
	ListenPlayer(*job_);
	state_->set_status(JobStatus::QUEUING);
	repo_.UpdateJobRuntime(*state_);
	Log().Debug() << "[MsgFlow] Start torque job, I'm job " << get_id();
	job_->GoPlay();
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
		repo_.UpdateJobRuntime(*state_);
		isover_ = true;
	}
}
