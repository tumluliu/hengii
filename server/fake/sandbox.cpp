/*
 * =====================================================================================
 *
 *       Filename:  sandbox.cpp
 *
 *    Description:  The test sand box
 *
 *        Version:  1.0
 *        Created:  04/23/2012 09:54:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>

extern "C" {
#include <pbs_ifl.h>
}

#include "recorder.h"
#include "speakingrecorder.h"
#include "player.h"
#include "speakingchainplayer.h"
#include "dbjobrepo.h"
#include "jobruntime.h"
#include "flowruntime.h"
#include "jobrepoentry.h"
#include "log.h"
#include "pbsattr.h"
#include "torquejob.h"
#include "tracker.h"
#include "jobtracker.h"
#include "cmdlinebuilder.h"
#include "trackerowner.h"

#include "hpgcjob_types.h"

using hpgc::higis::interface::JobFlow;
using hpgc::higis::interface::Job;
using std::string;

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  The entrance of the world
 * =====================================================================================
 */
int main() {
	//ConjunctionExp ce(3);
	//std::cout << "The value is: " << ce.Set(0, true) << std::endl;
	//std::cout << "The value is: " << ce.Set(1, true) << std::endl;
	//std::cout << "The value is: " << ce.Set(2, true) << std::endl;

	//std::vector<Player *> playlist;
	//ChainPlayer *pone;
	//ChainPlayer *ptwo;
	//ChainPlayer *pthree;

	//SpeakingRecorder spreco;
	//Recorder *reco = &spreco;

	//pone = new SpeakingChainPlayer(0);
	//playlist.push_back(pone);
	//ptwo = new SpeakingChainPlayer(2);
	//playlist.push_back(ptwo);
	//pthree = new SpeakingChainPlayer(5);
	//playlist.push_back(pthree);

	//std::vector<Player *> pres;
	//pres.push_back(pone);
	//pres.push_back(ptwo);

	//pthree->Listen(pres);

	//reco->Listen(playlist);

	//reco->LoadAll();

	//int sig;
	//std::cin >> sig;

	//delete pone;
	//delete ptwo;
	//delete pthree;

	//JobFlow flow;
	//Job first;
	//first.app_uri = "test/hello";
	//first.id = 0;
	//Job second;
	//second.app_uri = "test/hellooooo";
	//second.id = 1;
	//flow.jobs.push_back(first); 
	//flow.jobs.push_back(second);
	//flow.job_count = flow.jobs.size();

	//JobRepo *repo = JobRepoEntry::Open();

	//int64_t fid = repo->RegisterJobFlow(flow, "test");
	//FlowRuntime *nowall = repo->BorrowJobFlowRuntime(fid);

	//std::cout << nowall->get_jobruntime(0)->get_id() << std::endl;
	//JobRuntime *now = nowall->get_jobruntime_byid(0);

	//now->set_message("Aha!!!!");
	//now->set_status(JobStatus::FINISHED);
	//nowall->set_status(Status::FINISHED);
	//nowall->set_message("mmmmessage...");

	//repo->UpdateJobFlowRuntime(nowall);
	//repo->ReturnJobFlowRuntime(nowall);

	//Log().Fatal() << "Hello World!!";
	//Log().Error() << "Hello World!!";
	//Log().Warn() << "Hello World!!";
	//Log().Info() << "Hello World!!";
	//Log().Debug() << "Hello World!!";

	//const string host = "127.0.0.1";
	//const string path = "testsubmit";
	//int connection = pbs_connect(const_cast<char *>(host.c_str())); 
	//PbsAttr attr;
	//attr.Add(string(ATTR_N), "test");
	//attr.Add(string(ATTR_o), "aha.out");
	//attr.AddResource("nodes", "1:ppn=2");
	//char *ret = pbs_submit(connection, attr.MakeCAttrl(), const_cast<char *>(path.c_str()), 0, 0);
	//free(ret);
	//pbs_disconnect(connection);

	//TorqueJob job(2, "echo Hello World!");
	//job.Play();
	//Log().Info() << job.get_output();

	//JobFlow flow;
	//Job first;
	//first.app_uri = "";
	//first.id = 0;
	//flow.jobs.push_back(first); 
	//flow.job_count = flow.jobs.size();
	//int64_t id = JobRepoEntry::Open()->RegisterJobFlow(flow, "user");

	//FlowRuntime *state = JobRepoEntry::Open()->BorrowJobFlowRuntime(id);

	//Tracker flowT(state);
	//Player *realJob = new TorqueJob(2, "echo Hello World!");
	//JobTracker jobT(realJob, state->get_jobruntime_byid(0));
	//flowT.ListenPlayer(&jobT);

	//flowT.Play();

	//Log().Info() << "The output is: " << state->get_jobruntime_byid(0)->get_message();
	//jobT.ClearPlaylist();
	//delete realJob;

	//JobRepoEntry::Open()->ReturnJobFlowRuntime(state);

	//JobRepoEntry::Close();

	//Job first;
	//first.app_uri = "hello_to";
	//first.id = 0;
	//first.app_options.insert(
	//std::map<string, string>::value_type("target", "Anran"));
	//first.app_options.insert(
	//std::map<string, string>::value_type("prefix", "YANG"));
	//first.app_options.insert(
	//std::map<string, string>::value_type("coloredegg", "Aha"));
	//first.runtime_context.parallel_env 
	//= hpgc::higis::interface::ParallelEnv::MPI;
	//first.runtime_context.options.insert(
	//std::map<string, string>::value_type("process_count", "2"));

	//CmdlineBuilder builder;
	//string cmdline;
	//builder.Build(cmdline, first);
	//Log().Info() << cmdline;

	//JobFlow flow;
	//Job first;
	//first.app_uri = "hello_to";
	//first.id = 0;
	//first.app_options.insert(
			//std::map<string, string>::value_type("target", "Anran"));
	//first.app_options.insert(
			//std::map<string, string>::value_type("prefix", "YANG"));
	//first.app_options.insert(
			//std::map<string, string>::value_type("coloredegg", "Aha"));
	//first.runtime_context.parallel_env 
		//= hpgc::higis::interface::ParallelEnv::MPI;
	//first.runtime_context.options.insert(
			//std::map<string, string>::value_type("process_count", "2"));

	//flow.jobs.push_back(first); 
	//flow.job_count = flow.jobs.size();

	//int64_t id;
	//TrackerOwner owner(flow, "user");
	//Tracker* flowT = owner.Borrow(id);

	//flowT->Play();

	//FlowRuntime *state = JobRepoEntry::Open()->BorrowJobFlowRuntime(id);
	//Log().Info() << "The output is: " << state->get_jobruntime(0)->get_message();
	//JobRepoEntry::Open()->ReturnJobFlowRuntime(state);

	//owner.Return(flowT);

	//JobRepoEntry::Close();
	
	//Log().Info() << static_cast<DbJobRepo *>(JobRepoEntry::Open())->HasConflictFlowId(1336635379618);

	return 0;
}				/* ----------  end of function main  ---------- */
