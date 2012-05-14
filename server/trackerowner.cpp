/*
 * =====================================================================================
 *
 *       Filename:  trackerowner.cpp
 *
 *    Description:  build tracker from job flow
 *
 *        Version:  1.0
 *        Created:  05/09/2012 09:10:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "trackerowner.h"
#include "torquejob.h"
#include "jobrepoentry.h"
#include "cmdlinebuilder.h"
#include "log.h"
#include "utility.h"
#include "optionvalidator.h"
#include "programvalidator.h"

using std::string;
using hpgc::higis::interface::JobFlow;

TrackerOwner::TrackerOwner(const JobFlow & flow, const string & user) 
	: user_(user), flow_(flow), product_(NULL), 
	jobtrackers_(), batchjobs_(), state_(NULL), isborrowed_(false) {
	}

/* WARNING: not threadsafe!! by YANG Anran @ 2012.5.10 */
Tracker *TrackerOwner::Borrow(int64_t &id) {
	if (isborrowed_) {
		Log().Warn() << "The tracker " 
			<< product_->get_id() << " has already been borrowed.";
		id = -1;
		return NULL;
	}

	id = JobRepoEntry::Open()->RegisterJobFlow(flow_, user_);
	state_ = JobRepoEntry::Open()->BorrowJobFlowRuntime(id);

	if(Validate() != 0) { return NULL; }
	product_ = new Tracker(state_);

	if(BuildJobTrackers(id) != 0) { return NULL; }
	product_->Listen(jobtrackers_);

	LinkJobTrackers();

	isborrowed_ = true;
	return product_;
}

int TrackerOwner::Validate() {
	OptionValidator stage2;
	ProgramValidator stage1(&stage2);
	Validator checker(stage1);

	std::string msg;
	if(!checker.Chanllenge(flow_, msg)) {
		Fail(msg);
		return -1;
	}
	return 0;
}

int TrackerOwner::BuildJobTrackers(int64_t flowid) {
	for (int i = 0; i < flow_.job_count; i++) {
		CmdlineBuilder cmdBuilder;
		string cmd;
		if (cmdBuilder.Build(cmd, flow_.jobs[i]) != 0) {
			Log().Error() << cmd;
			Fail(flow_.jobs[i].id, cmd);
			return -1;
		}

		int np = util::stoi(
				flow_.jobs[i].runtime_context.options["process_count"]);
		/* WARNING: THIS is a workaround which also contains a magic number 10000.
		 * And all will turn vulunerable when the job count is over 10000, which
		 * is rather... ulikely. But it is still an ugly workaround because it involves
		 * so many knowledges that is not expressed here.
		 * by YANG Anran, @ 2012.5.14 */
		int64_t id = flowid * 10000 + flow_.jobs[i].id;
		Player *batchjob 
			= new TorqueJob(id, np, cmd);

		batchjobs_.push_back(batchjob);

		JobTracker *jtracker = new JobTracker(batchjob, 
				state_->get_jobruntime_byid(flow_.jobs[i].id));

		jobtrackers_.push_back(jtracker);
	}

	return 0;
}

int TrackerOwner::LinkJobTrackers() {
	for (int i = 0; i < flow_.job_count; i++) {
		for (int j = 0; j < flow_.jobs[i].parent_count; j++) {
			static_cast<JobTracker *>(jobtrackers_[i])->ListenPlayer(
					product_->get_player(flow_.jobs[i].parents[j]));
			Log().Debug() << "add " << flow_.jobs[i].parents[j] 
				<< " to " << flow_.jobs[i].id;
		}
	}
	return 0;
}

void TrackerOwner::Return(Tracker *product) {
	if (product_ == NULL) {
		Log().Error() << "I'm a pool tracker owner. I have no tracker produced.";
		return;
	}
	if (product != product_) {
		Log().Error() << "I'm a tracker owner, The product is not mine!";
		return;
	}
	CleanUp();
	isborrowed_ = false;
}

void TrackerOwner::CleanUp() {
	JobRepoEntry::Open()->ReturnJobFlowRuntime(state_);
	delete product_;
	for (size_t i = 0; i < batchjobs_.size(); i++) {
		delete batchjobs_[i];
	}
	for (size_t i = 0; i < jobtrackers_.size(); i++) {
		delete jobtrackers_[i];
	}
}

void TrackerOwner::Fail(const string &msg) {
	state_->set_status(Status::FAILED);
	state_->set_message(msg);
	JobRepoEntry::Open()->UpdateFlowPartRuntime(state_);
	CleanUp();
}

void TrackerOwner::Fail(int id, const string &msg) {
	Fail("job " + util::intToString(id) + ": " + msg);
	CleanUp();
}
