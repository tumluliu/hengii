/*
 * =====================================================================================
 *
 *       Filename:  flowruntime.cpp
 *
 *    Description:  class contains runtime informantion of a job flow
 *
 *        Version:  1.0
 *        Created:  05/02/2012 11:19:33 AM
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

#include "flowruntime.h"
#include <cstdlib>

using std::string;

FlowRuntime::FlowRuntime(int64_t id) 
	: id_(id), status_(Status::RUNNING), message_(""), jobdetails_() {
	}

FlowRuntime::FlowRuntime(const FlowRuntime &obj) :
	id_(obj.id_), status_(obj.status_),
	message_(obj.message_), jobdetails_(obj.jobdetails_) {
	}

/* WARNING: Here the create/destroy pair are not balanced, i.e. FlowRuntime doesn't create its
 * jobruntime but destroy them. The problem may not appear because now 
 * all usage of FlowRuntime is through jobrepo, and considering the two together, 
 * the create/destroy are balanced by YANG Anran @ 2012.5.10 */
FlowRuntime::~FlowRuntime() {
	for (size_t i = 0; i < jobdetails_.size(); i++) {
		delete jobdetails_[i];
	}
}

void FlowRuntime::Copy(const FlowRuntime &obj) {
	this->status_ = obj.status_;
	this->message_ = obj.message_;
	for (size_t i = 0; i < jobdetails_.size(); i++) {
		jobdetails_[i] = obj.jobdetails_[i];
	}
}

int64_t FlowRuntime::get_id() const {
	return id_;
}

int FlowRuntime::get_jobcount() const {
	return jobdetails_.size();
}

Status::type FlowRuntime::get_status() const {
	return status_;
}

string FlowRuntime::get_message() const {
	return message_;
}

JobRuntime *FlowRuntime::get_jobruntime(int pos) const {
	return jobdetails_[pos];
}
 
JobRuntime *FlowRuntime::get_jobruntime_byid(int id) const {
	for (size_t i = 0; i < jobdetails_.size(); i++) {
		if (jobdetails_[i]->get_id() == id) {
			return jobdetails_[i];
		}
	}
	return NULL;
}

void FlowRuntime::set_status(const Status::type &status) {
	status_ = status;
}

void FlowRuntime::set_message(const string &msg) {
	message_ = msg;
}

FlowRuntime &FlowRuntime::operator=(const FlowRuntime &rhs) {
	Copy(rhs);
	return *this;
}

void FlowRuntime::add_jobruntime(JobRuntime *child) {
	jobdetails_.push_back(child);
}
