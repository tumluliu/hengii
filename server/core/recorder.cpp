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

Recorder::Recorder() : idpos_(), playstate_(0) {
}

Recorder::~Recorder() {
}

void Recorder::OnePlayerDone(int64_t id) {
	int pos;
	if ((pos = get_index(id)) == -1) {
		Log().Error() << "id " << id << " not named my player";
		return;
	}

	Log().Info() << "Player " << id << " at " << pos << " done!";

	EndOnePlayer(id);
	if (playstate_.Set(pos, true)) {
		AllPlayersDone();
	}
}

void Recorder::ListenPlayer(Player &player) {
	player.add_recorder(shared_from_this());
	idpos_.insert(std::pair<int64_t, int>(player.get_id(), playstate_.get_size()));
	playstate_.Concat(1);
}

void Recorder::ClearPlaylist() {
	idpos_.clear();
	playstate_.Clear();
}

int Recorder::get_index(int64_t id) const {
	auto matched = idpos_.find(id);
	if (matched == idpos_.end()) {
		return -1;
	}

	return matched->second;
}

int Recorder::get_player_num() const {
	return idpos_.size();
}
