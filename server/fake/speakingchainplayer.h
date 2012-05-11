/*
 * =====================================================================================
 *
 *       Filename:  speakingchainplayer.h
 *
 *    Description:  A Speaking ChainPlayer. concrete class.
 *
 *        Version:  1.0
 *        Created:  04/25/2012 04:21:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  SPEAKINGCHAINCHILD_H_
#define  SPEAKINGCHAINCHILD_H_

#include "irecorder.h"
#include "chainplayer.h"

/*
 * =====================================================================================
 *        Class:  SpeakingChainPlayer
 *  Description:  He is built in a chain, and print messages in each step
 * =====================================================================================
 */
class SpeakingChainPlayer : public ChainPlayer {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		SpeakingChainPlayer(int64_t id) :
			ChainPlayer(id) {} /* constructor */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		
	protected:
		/* ====================  ACTIONS       ======================================= */
		virtual void AllPlayersDone();
		virtual void PlayMySong();

	private:
		/* ====================  DATA MEMBERS  ======================================= */
}; /* -----  end of class SpeakingPlayer  ----- */

#endif
