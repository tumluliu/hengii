/*
 * =====================================================================================
 *
 *       Filename:  speakingchainplayer.cpp
 *
 *    Description:  A Speaking ChainPlayer. concrete class.
 *
 *        Version:  1.0
 *        Created:  04/25/2012 04:24:41 PM
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

#include "speakingchainplayer.h"

void SpeakingChainPlayer::PlayMySong() {
	std::cout << "I'm " << get_id() << ", start walk" << std::endl;
	sleep(1 + get_id());
}

void SpeakingChainPlayer::AllPlayersDone() {
	sleep(2);
	std::cout << "pres all done!" << std::endl;

	ChainPlayer::AllPlayersDone();
}
