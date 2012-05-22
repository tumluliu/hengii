/*
 * =====================================================================================
 *
 *       Filename:  trackerfactory.cpp
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
#include <algorithm>

#include "trackerfactory.h"
#include "torquejob.h"
#include "metarepo.h"
#include "log.h"
#include "utility.h"
#include "jobrepo.h"
#include "resources.h"
#include "batchjobfactory.h"
#include "datarepo.h"
#include "flowinfo.h"
#include "datarequest.h"
#include "hpgcjob_types.h"

using std::string;
using hpgc::higis::interface::JobFlow;

TrackerFactory::TrackerFactory(const JobFlow & flow, const string & user,
		JobRepo &repo, const BatchJobFactory &batchfactory, 
		const Validator &guard, MetaRepo &metarepo,
		DataRepo &datarepo) :
	user_(user), flow_(flow), repo_(repo), batchfactory_(batchfactory),
	guard_(guard), metarepo_(metarepo), datarepo_(datarepo) {
	}

std::shared_ptr<Tracker> TrackerFactory::Create(int64_t id) const {
	std::unique_ptr<FlowRuntime> state(repo_.CreateFlowRuntime(id));

	std::vector<std::shared_ptr<JobTracker> > jobtrackers;

	string msg;
	if(
			Validate(msg) != 0
			|| BuildJobTrackers(jobtrackers, msg, *state, id) != 0
			|| LinkJobTrackers(jobtrackers) != 0
			|| LockData(msg, id) != 0) {
		Fail(msg, *state);
		return std::shared_ptr<Tracker>();
	}

	
	std::shared_ptr<Tracker> pproduct(
			new Tracker(
				std::move(state), 
				repo_, 
				std::unique_ptr<TrackerEnv>(new TrackerEnv(datarepo_, id))
				));

	for (size_t i = 0; i < jobtrackers.size(); i++) {
		pproduct->add_jobtracker(jobtrackers[i]);
	}
	return pproduct;
}

int TrackerFactory::Validate(string &out_msg) const {
	if(!guard_.Chanllenge(flow_, out_msg)) {
		return -1;
	}
	return 0;
}

int TrackerFactory::BuildJobTrackers(std::vector<std::shared_ptr<JobTracker> > &out_jobtrackers,
		string &out_msg, FlowRuntime &state, int64_t flowid) const {
	for (int i = 0; i < flow_.job_count; i++) {
		string cmd;
		if (CmdlineBuilder(metarepo_).Build(cmd, flow_.jobs[i]) != 0) {
			out_msg = 
				"job " + util::intToString(flow_.jobs[i].id) 
				+ ": " + cmd;
			return -1;
		}

		int np;
		auto match = flow_.jobs[i].runtime_context.options.find("process_count");
		if (match != flow_.jobs[i].runtime_context.options.end()) {
			np = util::stoi(match->second);
		}
		else {
			np = 1;
		}
		/* WARNING: THIS is a workaround which also contains a magic number 10000.
		 * And all will turn vulunerable when the job count is over 10000, which
		 * is rather... ulikely. But it is still an ugly workaround because it involves
		 * so many knowledges that is not expressed here.
		 * by YANG Anran, @ 2012.5.14 */
		int64_t id = flowid * 10000 + flow_.jobs[i].id;

		Resources rsrc;
		rsrc.Request(np);
		out_jobtrackers.push_back(std::shared_ptr<JobTracker>(new JobTracker(
						batchfactory_.Create(id, rsrc, cmd), 
						state.get_jobruntime_byid(flow_.jobs[i].id),
						repo_)));
	}

	return 0;
}

int TrackerFactory::LinkJobTrackers(
		const std::vector<std::shared_ptr<JobTracker> > &jobtrackers) const {
	for (int i = 0; i < flow_.job_count; i++) {
		for (int j = 0; j < flow_.job_count; j++) {
			const std::vector<int32_t> &parents = flow_.jobs[i].parents;
			int32_t cid = flow_.jobs[i].id;
			int32_t pid = flow_.jobs[j].id;

			if (std::find(parents.begin(),
						parents.end(), pid) != parents.end()) {
				jobtrackers[cid]->ListenPlayer(*jobtrackers[pid]);
				Log().Debug() << "add " << flow_.jobs[i].parents[j] 
					<< " to " << flow_.jobs[i].id;
			}
		}
	}
	return 0;
}

void TrackerFactory::Fail(const string &msg,
		FlowRuntime &state) const {
	Log().Error() << msg;
	state.set_status(Status::FAILED);
	state.set_message(msg);
	repo_.UpdateFlowPartRuntime(state);
}

int TrackerFactory::LockData(string &out_msg, int64_t id) const {
	std::vector<DataRequest> reqlist;
	FlowInfo(flow_, metarepo_).get_datareqlist(reqlist);
	Log().Debug() << "req num is: " << reqlist.size();
	if (datarepo_.Lock(out_msg, id, reqlist) != 0) {
		return -1;
	}
	return 0;
}
