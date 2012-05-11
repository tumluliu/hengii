/*
 * =====================================================================================
 *
 *       Filename:  speakingplayer.cpp
 *
 *    Description:  Fake class. A class to test Player base class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 03:14:12 PM
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

#include "speakingplayer.h"

SpeakingPlayer::~SpeakingPlayer() {
}

void SpeakingPlayer::Play() {
	std::cout << "I'm " << get_id() << ", Go!!" << std::endl;
	sleep(1 + get_id());
	for (int i = 0; i < get_recorders_num(); i++) {
		get_recorder(i)->OnePlayerDone(get_id());
	}
}
