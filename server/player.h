/*
 * =====================================================================================
 *
 *       Filename:  player.h
 *
 *    Description:  The interface of the main working class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 08:53:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef  CHILD_H_
#define  CHILD_H_

#include <string>
#include <pthread.h>

#include "irecorder.h"

/* easy send message to all recorders */
#define EACH_RECORDER(msg) { for (int i = 0; i < get_recorders_num(); i++) { \
                           	   IRecorder *recorder = get_recorder(i); \
	                           recorder->msg; \
                           }}

/*
 * =====================================================================================
 *        Class:  Player
 *  Description:  He/She start a music, report every event to his/her recorder,
 *                until the endpoint is reached.
 * =====================================================================================
 */
class Player {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* input 1:id, 2:recorder */
		Player();
		Player(int64_t);                             /* constructor */
		virtual ~Player();
		Player(const Player &);

		/* ====================  ACCESSORS     ======================================= */
		int64_t get_id() const;
		/* default implementation given for empty output player */
		virtual std::string get_output() const;

		/* ====================  MUTATORS      ======================================= */
		void add_recorder(IRecorder *);

		/* ====================  ACTIONS       ======================================= */
		/* start to do something. asynchronous. */
		void GoPlay();
		/* do something. synchronous. */
		virtual void Play() = 0;

		/* ====================  OPERATORS     ======================================= */
		Player &operator=(const Player &);

	protected:
		/* ====================  ACCESSORS     ======================================= */
		IRecorder *get_recorder(int) const;
		int get_recorders_num() const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		int64_t id_;
		std::vector<IRecorder*> recorders_;
		bool isplaying_;

		/* ====================  ACTIONS       ======================================= */
		/* The thread function, asynchronous part of method 'GoPlay' */
		static void *Process(void *);
}; /* -----  end of class Player  ----- */

#endif
