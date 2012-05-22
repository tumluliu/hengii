/*
 * =====================================================================================
 *
 *       Filename:  appoption.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/2012 11:14:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "appoption.h"
#include "utility.h"

using std::vector;
using std::string;

AppOption::AppOption(const std::string &name, bool isopt, const std::string &cmdswitch,
		const std::string &defaultvalue) : 
	name_(name), isoptional_(isopt), cmdswitch_(cmdswitch), 
	defaultvalue_(defaultvalue), extra_() {
	}

AppOption::AppOption() : name_(""), isoptional_(true), cmdswitch_(""), 
	defaultvalue_(""), extra_() {}

	std::string AppOption::get_name() const {
		return name_;
	}

bool AppOption::IsOptional() const {
	return isoptional_;
}

std::string AppOption::get_cmdswitch() const {
	return cmdswitch_;
}

std::string AppOption::get_defaultvalue() const {
	return defaultvalue_;
}

void AppOption::set_name(const std::string &name) {
	name_ = name;
}

void AppOption::set_optional(bool isoptional) {
	isoptional_ = isoptional;
}

void AppOption::set_cmdswitch(const std::string &cmdswitch) {
	cmdswitch_ = cmdswitch;
}

void AppOption::set_defaultvalue(const std::string &defaultvalue) {
	defaultvalue_ = defaultvalue;
}

int AppOption::get_extra(std::string &out_value, const std::string &key) const {
	auto matched = extra_.find(key);
	if (matched != extra_.end()) {
		out_value = matched->second;
		return 0;
	}
	else {
		out_value = "";
		return -1;
	}
}

void AppOption::add_extra(const std::string &key, const std::string &value) {
	extra_.insert(std::pair<string, string>(key, value));
}
