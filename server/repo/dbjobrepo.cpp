/*
 * =====================================================================================
 *
 *       Filename:  dbjobrepo.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/08/2012 03:11:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani()
 *         Updator: LIU Lu() YANG Anran()
 *   Organization:
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "dbjobrepo.h"
#include "utility.h"
#include "log.h"
#include "hpgcjob_types.h"
#include "config.h"

using hpgc::higis::interface::JobFlow;
using std::string;
using std::vector;

DbJobRepo::DbJobRepo(SqlDb &db) : db_(db), registerlock_() {
	pthread_mutex_init(&registerlock_, NULL);
}

DbJobRepo::~DbJobRepo() {
	pthread_mutex_destroy(&registerlock_);
}

int64_t DbJobRepo::RegisterJobFlow(const JobFlow &data, const string &user) {
	pthread_mutex_lock(&registerlock_);
	int64_t fid = GenerateFlowId();

	RegisterJobFlowToDb(fid, user);
	for (int i = 0; i < data.job_count; i++) {
		RegisterJobToDb(fid, data.jobs[i].id, data.jobs[i].app_uri);
	}
	pthread_mutex_unlock(&registerlock_);

	return fid;
}

void DbJobRepo::UpdateJobFlowRuntime(const FlowRuntime &data) {
	UpdateFlowPartRuntime(data);
	for (int i = 0; i < data.get_jobcount(); i++) {
		UpdateJobRuntime(*data.get_jobruntime(i));
	}
}

void DbJobRepo::UpdateFlowPartRuntime(const FlowRuntime &data) {
	UpdateJobFlowStatus(data.get_id(), data.get_status(), data.get_message());
}

void DbJobRepo::UpdateJobRuntime(const JobRuntime &data) {
	Log().Debug() 
		<< "update job runtime, fid is:" << data.get_fid()
		<< ", id is: " << data.get_id()
		<< ", status is: " << data.get_status()
		<< ", message is: " << data.get_message();
	UpdateJobStatus(data.get_fid(), data.get_id(), data.get_status(), data.get_message());
}

const string DbJobRepo::GetConflictFlowIdSql(int64_t id) const {
	std::stringstream ss;
	ss << "select count(*) from " << JOB_FLOW_TABLE_NAME 
		<< " where id = " << id << ";";
	return ss.str();
}

int64_t DbJobRepo::GenerateFlowId() {
	int64_t id;
	int64_t prefix;

	srand(time(0));
	prefix = time(0) * 1000;
	id = prefix + rand() % 1000;

	while (HasConflictFlowId(id)) {
		id += 1;
	}

	return id;
}

bool DbJobRepo::HasConflictFlowId(int64_t id) const {
	if (db_.QuerySingle(GetConflictFlowIdSql(id)) == "0") {
		Log().Debug() << "conflict id num is: " << db_.QuerySingle(GetConflictFlowIdSql(id));
		return false;
	}
	else {
		return true;
	}
}

std::unique_ptr<FlowRuntime> DbJobRepo::CreateFlowRuntime(int64_t id) { 
	std::unique_ptr<FlowRuntime> fp(new FlowRuntime(id));
	std::string msg;
	int status = GetFlowStatus(id, msg);
	if (status < 0) {
		Log().Info() << "No flow runtime " << id << " found.";
		return std::unique_ptr<FlowRuntime>();
	}

	Status::type stat = static_cast<Status::type>(status);
	fp->set_status(stat);
	fp->set_message(msg);

	std::vector<int> jobids;
	GetJobIds(id, jobids);

	for (std::vector<int>::const_iterator i = jobids.begin();
			i != jobids.end(); i++) {
		std::shared_ptr<JobRuntime> jp(new JobRuntime(id, *i));
		std::string jobmsg;
		JobStatus::type jobstat = static_cast<JobStatus::type>(GetJobStatus(id, *i, jobmsg));
		jp->set_status(jobstat);
		jp->set_message(jobmsg);
		fp->add_jobruntime(jp);
	}
	return fp;
}

const string DbJobRepo::RegisterJobSql( int64_t flowId, int jobId, const string &appUri ) const {
	std::stringstream ss;
	ss << "insert into " << JOB_TABLE_NAME 
		<< "(fid, id, status, appuri) values('" << flowId
		<< "', '" << jobId << "', '" 
		<< hpgc::higis::interface::JobStatus::WAITING_FOR_SUBMIT << "', '"
		<< appUri << "');";
	return ss.str(); 
}

int DbJobRepo::RegisterJobToDb(int64_t flowId, int jobId, const string &appUri) {
	return db_.Command( RegisterJobSql( flowId, jobId, appUri ) );
}

const string DbJobRepo::RegisterJobFlowSql( int64_t flowId, const string &userId ) const {
	std::stringstream ss;
	ss << "insert into " << JOB_FLOW_TABLE_NAME 
		<< "(id, user, status, ctime, utime) values('" 
		<< util::intToString( flowId ) << "', '" 
		<< userId << "', '" 
		<< hpgc::higis::interface::Status::RUNNING << "', '"
		<< util::getCurrentTime() << "', '" 
		<< util::getCurrentTime() << "');";
	return ss.str();
}

int DbJobRepo::RegisterJobFlowToDb( int64_t flowId, const string &userId ) {
	return db_.Command( RegisterJobFlowSql( flowId, userId ) );
}

void DbJobRepo::EscapeSingleQuote( string &origin ) const {
	std::size_t current = 0;
	while(current < origin.size() && (current = origin.find('\'', current)) != string::npos) {
		origin.replace(current, 1, "''");
		current += 2; // move from the head of '' to the end of ''
	}
}

const string DbJobRepo::UpdateJobStatusSql( int64_t flowId, int jobId, int status, const string &message ) const {
	string msg(message);
	EscapeSingleQuote(msg);
	return "update " + JOB_TABLE_NAME + " set status=" + util::intToString( status ) 
		+ ", message='" + msg
		+ "' where fid='" + util::intToString( flowId ) 
		+ "' and id='" + util::intToString( jobId ) + "';";
}

int DbJobRepo::UpdateJobStatus( int64_t flowId, int jobId, int status, const string &output ) {
	return db_.Command( UpdateJobStatusSql( flowId, jobId, status, output ) );
}


const string DbJobRepo::UpdateJobFlowStatusSql(int64_t flowId, int status, const string &message) const {
	string msg(message);
	EscapeSingleQuote(msg);
	return "update " + JOB_FLOW_TABLE_NAME 
		+ " set status=" + util::intToString( status ) 
		+ ", message='" + msg 
		+ "', utime='" + util::getCurrentTime()
		+ "' where id='" + util::intToString(flowId) + "'";
}

int DbJobRepo::UpdateJobFlowStatus(int64_t flowId, int status, const string &message) {
	return db_.Command( UpdateJobFlowStatusSql(flowId, status, message) );
}

const string DbJobRepo::GetJobStatusSql( int64_t flowId, int jobId ) const {
	return "select status, message from " + JOB_TABLE_NAME
		+ " where fid='" + util::intToString( flowId )
		+ "' and id='" + util::intToString( jobId ) + "';";
}

const string DbJobRepo::GetJobCountSql( int64_t flowId ) const {
	return "select count(*) as job_count from " + JOB_TABLE_NAME
		+ " where fid='" + util::intToString( flowId )
		+ "';";
}

const string DbJobRepo::GetFlowStatusSql( int64_t flowId ) const {
	return "select status, message from " + JOB_FLOW_TABLE_NAME
		+ " where id='" + util::intToString( flowId ) + "';";
}

const string DbJobRepo::GetJobIdsSql( int64_t flowId ) const {
	return "select id from " + JOB_TABLE_NAME
		+ " where fid='" + util::intToString( flowId ) + "';";
}

int DbJobRepo::GetJobStatus( int64_t flowId, int jobId, string &message ) {
	int status;
	vector<string> res;

	db_.QueryRow( res, GetJobStatusSql( flowId, jobId ) );
	if (res.size() == 0) {
		return -1;
	}

	status = util::stoi(res[0]);
	message = res[1];

	return status;
}

void DbJobRepo::GetJobIds( int64_t flowId, std::vector<int> &ids ) { 
	std::vector<string> texts;
	db_.QueryColumn(texts, GetJobIdsSql( flowId ));
	ids.resize(texts.size());
	transform(texts.begin(), texts.end(), ids.begin(), util::stoi);
}

/* TO be changed*/
int DbJobRepo::GetJobCount( int64_t flowId ) {
	vector<string> res;
	db_.QueryRow( res, GetJobCountSql( flowId ) );
	return util::stoi(res[0]);
}

int DbJobRepo::GetFlowStatus( int64_t flowId, string &message ) {
	int status;
	vector<string> res;

	db_.QueryRow( res, GetFlowStatusSql( flowId ) );
	if (res.size() == 0) {
		return -1;
	}

	message = res[1];
	status = util::stoi(res[0]);

	return status;
}
