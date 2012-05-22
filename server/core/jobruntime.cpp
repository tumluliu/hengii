/*
 * =====================================================================================
 *
 *       Filename:  jobruntime.cpp
 *
 *    Description:  class contains runtime informantion of a job
 *
 *        Version:  1.0
 *        Created:  05/02/2012 10:57:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string>

#include "jobruntime.h"

using std::string;

JobRuntime::JobRuntime(int64_t fid, int id) 
	: id_(id), fid_(fid), status_(JobStatus::WAITING_FOR_SUBMIT), message_("") {
	}

JobRuntime::JobRuntime(const JobRuntime &obj) 
	: id_(obj.id_), fid_(obj.fid_), status_(obj.status_), message_(obj.message_) {
}

void JobRuntime::Copy(const JobRuntime &obj) {
	this->id_ = obj.id_;
	this->fid_ = obj.fid_;
	this->status_ = obj.status_;
	this->message_ = obj.message_;
}

int JobRuntime::get_id() const {
	return id_;
}

int64_t JobRuntime::get_fid() const {
	return fid_;
}

JobStatus::type JobRuntime::get_status() const {
	return status_;
}

string JobRuntime::get_message() const {
	return message_;
}

void JobRuntime::set_status(const JobStatus::type &status) {
	status_ = status;
}

void JobRuntime::set_message(const string &msg) {
	message_ = msg;
}

JobRuntime &JobRuntime::operator=(const JobRuntime &rhs) {
	Copy(rhs);
	return *this;
}
