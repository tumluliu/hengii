/*
 * =====================================================================================
 *
 *       Filename:  speakingplayer.h
 *
 *    Description:  Fake class. A class to test Player base class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 03:11:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef  SPEAKINGCHILD_H_
#define  SPEAKINGCHILD_H_

#include "player.h"

/*
 * =====================================================================================
 *        Class:  SpeakingPlayer
 *  Description:  This class print a message in each step as a player
 * =====================================================================================
 */
class SpeakingPlayer : public Player {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* constructor */
		SpeakingPlayer(int id) : Player(id) {}
		virtual ~SpeakingPlayer();

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

		/* ====================  ACTIONS       ======================================= */
		/* do something. synchronous. */
		void Play();

	protected:

	private:
		/* ====================  DATA MEMBERS  ======================================= */
}; /* -----  end of class SpeakingPlayer  ----- */

#endif
