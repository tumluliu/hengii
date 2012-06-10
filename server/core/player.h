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
#include <memory>
#include <pthread.h>

#include "irecorder.h"

/* easy send message to all recorders */
#define EACH_RECORDER(msg) \
{ for (int i = 0; i < get_recorders_num(); i++) { \
	if( auto recorder = get_recorder(i).lock()) { \
		recorder->msg; \
	}}}

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
		Player(int64_t);                             /* constructor */
		virtual ~Player();

		/* ====================  ACCESSORS     ======================================= */
		int64_t get_id() const;
		/* default implementation given for empty output player */
		virtual std::string get_output() const;

		/* ====================  MUTATORS      ======================================= */
		/* WARNING: Here bossrecorde seems not very generalised, maybe recorders with
		 * role, or recorder groups are more suitable and can be implemented as 
		 * composite pattern. However, it seems a overkill till now.
		 * by YANG Anran @ 2012.5.24 */
		void set_bossrecorder(std::weak_ptr<IRecorder>);
		void add_recorder(std::weak_ptr<IRecorder>);

		/* ====================  ACTIONS       ======================================= */
		/* start to do something. asynchronous. */
		void GoPlay();
		virtual void Stop() = 0;

		/* ====================  OPERATORS     ======================================= */

	protected:
		/* ====================  ACCESSORS     ======================================= */
		std::weak_ptr<IRecorder> get_bossrecorder() const;
		std::weak_ptr<IRecorder> get_recorder(int) const;
		int get_recorders_num() const;

		/* ====================  ACTIONS       ======================================= */
		/* do something. synchronous. */
		virtual void Play() = 0;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		int64_t id_;
		std::weak_ptr<IRecorder> bossrecorder_;
		std::vector<std::weak_ptr<IRecorder> > recorders_;
		bool isplaying_;

		/* ====================  ACTIONS       ======================================= */
		/* The thread function, asynchronous part of method 'GoPlay' */
		static void *Process(void *);

		/* ====================  DISABLED      ======================================= */
		Player(const Player &);
		Player &operator=(const Player &);
}; /* -----  end of class Player  ----- */

#endif
