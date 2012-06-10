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
: Player(state->get_id()), Recorder(), job_(std::move(job)), state_(std::move(state)), 
	repo_(repo), myturn_(false), cancel_(false) {
	}

JobTracker::~JobTracker() {}

void JobTracker::Play() {
	if (get_player_num() == 0) {
		myturn_ = true;
	}
	while(!myturn_) { 
		if (cancel_) {
			Log().Debug() << "[MsgFlow] job canceled, I'm job " << get_id();
			UpdateStatusAndNotify(JobStatus::CANCELED);
		}
	}

	ClearPlaylist();
	ListenPlayer(*job_, true);
	
	UpdateStatusAndNotify(JobStatus::RUNNING);

	Log().Debug() << "[MsgFlow] Start torque job, I'm job " << get_id();
	job_->GoPlay();
}

void JobTracker::AllPlayersDone() {
	if (!myturn_) {
		Log().Debug() << "[MsgFlow] Parents jobs complete, I'm job " << get_id();
		myturn_ = true;
	}
	else {
		/* handle */
		Log().Debug() << "[MsgFlow] Torque job complete, I'm job " << get_id();
		Log().Info() << "job complete, result is: " << job_->get_output();
		state_->set_message(job_->get_output());
		UpdateStatusAndNotify(JobStatus::FINISHED);
	}
}

void JobTracker::AllPlayersCanceled() {
	state_->set_message(job_->get_output());
	UpdateStatusAndNotify(JobStatus::CANCELED);
}

void JobTracker::Stop() {
	if (!myturn_) {
		cancel_ = true;
	}
	else {
		cancel_ = true;
		Log().Debug() << "[MsgFlow] Cancel torque job, I'm job " << get_id();
		job_->Stop();
	}
}

void JobTracker::UpdateStatusAndNotify(JobStatus::type status) {
	state_->set_status(status);
	repo_.UpdateJobRuntime(*state_);

	if (auto boss = get_bossrecorder().lock()) {

		switch ( status ) {
			case JobStatus::FINISHED:	
				EACH_RECORDER(OnePlayerDone(get_id()));
				boss->OnePlayerDone(get_id());
				break;

			case JobStatus::CANCELED:	
				boss->OnePlayerCanceled(get_id());
				break;

			default:	
				break;
		}				/* -----  end switch  ----- */
	}
}
