/*
 * =====================================================================================
 *
 *       Filename:  recorder.h
 *
 *    Description:  The interface of the observer class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 08:54:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef  MOTHER_H_
#define  MOTHER_H_

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <vector>
#include <pthread.h>

#include "irecorder.h"
#include "player.h"
#include "conjunctionexp.h"

/*
 * =====================================================================================
 *        Class:  Recorder
 *  Description:  She prepare, start, observe several players's journey, and do some
 *                post work when they all have done
 * =====================================================================================
 */
class Recorder : public IRecorder {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* input: 1:players ids */
		Recorder();                             /* constructor */
		virtual ~Recorder();

		/* ====================  INITIALIZERS  ======================================= */
		void Listen(const std::vector<Player *>);
		void ListenPlayer(Player *);
		/* WARNING: This is not THREADSAFE!!
		 * Please use it carefully! by YANG Anran @ 2012.5.10 */
		void ClearPlaylist();

		/* ====================  ACTIONS       ======================================= */
		void LoadAllPlayers();

		/* ====================  EVENTS        ======================================= */
		/* input: 1:the done player id */
		virtual void OnePlayerDone(int64_t);

		/* ====================  ACCESSORS     ======================================= */
		Player *get_player(int64_t) const;
		int get_player_num() const;

	protected:
		/* ====================  ACTIONS       ======================================= */
		/* input: 1:the id of player to be ended */
		virtual void EndOnePlayer(int64_t) {}
		/* ====================  EVENTS        ======================================= */
		virtual void AllPlayersDone() = 0;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		/* player i is finished is equally to playstate[i] == true,
		 * and vice versa */
		std::vector<Player *> playlist_; 
		ConjunctionExp playstate_;
		pthread_mutex_t lock_;

		/* ====================  ACCESSORS     ======================================= */
		/* return the order of player with given id
		 * input: id
		 * return: order */
		int get_index(int64_t) const;

}; /* -----  end of class Recorder  ----- */

#endif
