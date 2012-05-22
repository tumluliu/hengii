/*
 * =====================================================================================
 *
 *       Filename:  chainplayer.cpp
 *
 *    Description:  player walking in a chain
 *
 *        Version:  1.0
 *        Created:  04/22/2012 02:45:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "chainplayer.h"
#include "log.h"

ChainPlayer::~ChainPlayer() {}

void ChainPlayer::AllPlayersDone() {
	myturn_ = true;
}

void ChainPlayer::Play() {
	if (get_player_num() == 0) {
		myturn_ = true;
	}
	while(!myturn_) { }
	PlayMySong();
	Log().Debug() << "[MsgFlow] Job " << get_id() << " complete";
	EACH_RECORDER(OnePlayerDone(get_id()));
}

bool ChainPlayer::IsMyTurn() const {
	return myturn_;
}
