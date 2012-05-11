/*
 * =====================================================================================
 *
 *       Filename:  recorder.cpp
 *
 *    Description:  The interface of the observer class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 08:57:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "recorder.h"
#include "log.h"

Recorder::Recorder() : playlist_(), playstate_(0), lock_() {
	pthread_mutex_init(&lock_, NULL);
}

Recorder::~Recorder() {
	pthread_mutex_destroy(&lock_);
}

void Recorder::OnePlayerDone(int64_t id) {
	EndOnePlayer(id);
	if (playstate_.Set(
				get_index(id),
				true)) {
		AllPlayersDone();
	}
}

void Recorder::Listen(std::vector<Player *> orphans) {
	for (size_t i = 0; i < orphans.size(); i++) {
		ListenPlayer(orphans[i]);
	}
}

void Recorder::ListenPlayer(Player *player) {
	player->add_recorder(this);
	playlist_.push_back(player);
	playstate_.Concat(1);
}

void Recorder::ClearPlaylist() {
	playlist_.clear();
	playstate_.Clear();
}

void Recorder::LoadAllPlayers() {
	size_t i;

	for (i = 0; i < playlist_.size(); i++) {
		playlist_[i]->GoPlay();
	}
}

int Recorder::get_index(int64_t id) const {
	size_t i;

	for (i = 0; i < playlist_.size(); i++) {
		if (playlist_[i]->get_id() == id) {
			return i;
		}
	}

	return -1;
}

Player *Recorder::get_player(int64_t id) const {
	return playlist_[get_index(id)];
}

int Recorder::get_player_num() const {
	return playlist_.size();
}
