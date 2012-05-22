/*
 * =====================================================================================
 *
 *       Filename:  chainplayer.h
 *
 *    Description:  player playing in a chain
 *
 *        Version:  1.0
 *        Created:  04/22/2012 02:42:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  CHAINCHILD_H_
#define  CHAINCHILD_H_

#include <vector>

#include "recorder.h"
#include "player.h"

/*
 * =====================================================================================
 *        Class:  ChainPlayer
 *  Description:  They play one by one, or one after serveral, or serveral after one
 * =====================================================================================
 */
class ChainPlayer : public Player, public Recorder
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		ChainPlayer (int64_t id) : 
			Player(id), Recorder(), myturn_(false) {} /* constructor */
		virtual ~ChainPlayer();

		/* ====================  ACTIONS       ======================================= */
		/* do something. synchronous. */
		virtual void Play();

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

	protected:
		/* ====================  ACTIONS       ======================================= */
		/* As player */
		virtual void PlayMySong() = 0;

		/* As Recorder */
		virtual void AllPlayersDone();

		/* ====================  HELPERS       ======================================= */
		bool IsMyTurn() const;

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		bool myturn_;

}; /* -----  end of class ChainPlayer  ----- */

#endif
