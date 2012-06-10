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
#include <pthread.h>
#include <vector>
#include <map>
/* WARNING: gcc only. I cannot suppress warnings from system headers. This class should
 * be examined more carefully cause it's no protection of -Weffc++ */
#pragma GCC diagnostic ignored "-Weffc++"
#include <memory>

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
class Recorder : public IRecorder, public std::enable_shared_from_this<Recorder> {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* input: 1:players ids */
		Recorder();                             /* constructor */
		virtual ~Recorder();

		/* ====================  INITIALIZERS  ======================================= */
		void ListenPlayer(Player &);
		/* args: player;if as boss */
		void ListenPlayer(Player &, bool);

		/* NOTICE: This is not THREADSAFE!!
		 * Please use it carefully! by YANG Anran @ 2012.5.10 */
		void ClearPlaylist();

		/* ====================  EVENTS        ======================================= */
		/* input: 1:the done player id */
		virtual void OnePlayerDone(int64_t);
		/* input: 1:the canceled player id */
		virtual void OnePlayerCanceled(int64_t);

		/* ====================  ACCESSORS     ======================================= */
		int get_player_num() const;

	protected:
		/* ====================  EVENTS        ======================================= */
		virtual void AllPlayersDone() = 0;
		virtual void AllPlayersCanceled() = 0;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		/* The convienience dict to lookup player pos in playstate_ accoring to its id */
		std::map<int64_t, int> idpos_;
		/* that player i is terminated(finished, canceled, failed) is equally to 
		 * playstate[i] == true, and vice versa */
		ConjunctionExp playstate_;

		/* ====================  ACCESSORS     ======================================= */
		/* return the order of player with given id
		 * input: id
		 * return: order */
		int get_index(int64_t) const;

}; /* -----  end of class Recorder  ----- */

#endif
