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

Resources::Resources() : nodes_(0), ppn_(0) {
}

int Resources::get_nodes() { 
	return nodes_; 
}

int Resources::get_ppn() {
	return ppn_;
}

void Resources::Request(int processes) {
	if (processes == 1) {
		nodes_ = 1;
		ppn_ = 1;
	}
	else {
		nodes_ = NODENUM;
		ppn_ = processes / NODENUM;
	}
}
