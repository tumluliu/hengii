/*
 * =====================================================================================
 *
 *       Filename:  status.h
 *
 *    Description:  job and job flow status, sync with ones in interface, 
 *                  redefine here to decouple
 *
 *        Version:  1.0
 *        Created:  05/07/2012 05:00:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  STATUS_H_
#define  STATUS_H_

struct JobStatus {
	enum type {
		FINISHED = 1,
		RUNNING = 2,
		WAITING_FOR_SUBMIT = 3,
		QUEUING = 4,
		FAILED = 5,
		NOT_EXIST = 6,
		PAUSED = 7
	};
};

struct Status {
	enum type {
		FINISHED = 1,
		RUNNING = 2,
		FAILED = 3,
		NOT_EXIST = 4,
		PAUSED = 5
	};
};

#endif
