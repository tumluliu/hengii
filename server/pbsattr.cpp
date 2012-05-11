/*
 * =====================================================================================
 *
 *       Filename:  pbsattr.cpp
 *
 *    Description:  pbs attr builder
 *
 *        Version:  1.0
 *        Created:  05/06/2012 08:19:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <cstring>

#include "pbsattr.h"

using std::map;
using std::string;

PbsAttr::PbsAttr() : params_(), resources_(), cache_(NULL) {
}

PbsAttr::~PbsAttr() {
	DestroyCAttrl(cache_);
}

void PbsAttr::Add(const char *name, const string &value) {
	params_.insert(map<string, string>::value_type(string(name), value));
}

void PbsAttr::AddResource(const string &name, const string &value) {
	resources_.insert(map<string, string>::value_type(name, value));
}

struct attropl *PbsAttr::MakeCAttrl() {
	// clear cache to avoid memory leak
	DestroyCAttrl(cache_);

	// The head element is not included in the result, only to 
	// acheive a fluent code style
	struct attropl head;
	struct attropl *last = &head;

	for (map<string, string>::const_iterator i = params_.begin();
			i != params_.end(); i++) {
		last = AddCAttrEle(last, i->first, "", i->second);
	}
	for (map<string, string>::const_iterator i = resources_.begin();
			i != resources_.end(); i++) {
		last = AddCAttrEle(last, string(ATTR_l), i->first, i->second);
	}

	cache_ = head.next;

	return cache_;
}

struct attropl *PbsAttr::AddCAttrEle(struct attropl *last, 
		const string &name, const string &resource,
		const string &value) {
	struct attropl *newele = (struct attropl *)malloc(sizeof(struct attropl));

	newele->name = (char *)malloc(name.size() + 1);
	strncpy(newele->name, name.c_str(), name.size() + 1);

	newele->value = (char *)malloc(value.size() + 1);
	strncpy(newele->value, value.c_str(), value.size() + 1);

	if (resource != "") {
		newele->resource = (char *)malloc(resource.size() + 1);
		strncpy(newele->resource, resource.c_str(), resource.size() + 1);
	}
	else {
		newele->resource = (char *)NULL;
	}

	newele->next = NULL;

	last->next = newele;
	return newele;
}

void PbsAttr::DestroyCAttrl(struct attropl *attr) {
	struct attropl *p = attr;
	struct attropl *temp = attr;

	if ( p == NULL ) {
		return;
	}

	do {
		free(p->name);
		free(p->value);
		if (p->resource != NULL) { free(p->resource); }
		temp = p;
		p = p->next;
		free(temp);
	} while ( p != NULL );
}
