/*
 * =====================================================================================
 *
 *       Filename:  resources.cpp
 *
 *    Description:  pbs resources
 *
 *        Version:  1.0
 *        Created:  05/07/2012 10:26:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "resources.h"
#include "config.h"

Resources::Resources() : nodes_(0), ppn_(0) {
}

int Resources::get_nodes() const { 
	return nodes_; 
}

int Resources::get_ppn() const {
	return ppn_;
}

void Resources::Request(int processes) {
	if (processes <= NODENUM) {
		nodes_ = processes;
		ppn_ = 1;
	}
	else {
		nodes_ = NODENUM;
		ppn_ = processes / NODENUM;
	}
	/* WARNING: Need a checking to prevent ppn exceeds cpus per nodes. 
	 * by YANG Anran @ 2012.5.15 */
}
