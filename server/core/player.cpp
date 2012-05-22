/*
 * =====================================================================================
 *
 *       Filename:  player.cpp
 *
 *    Description:  The interface of the main working class
 *
 *
 *        Version:  1.0
 *        Created:  04/23/2012 08:49:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "player.h"

Player::Player(int64_t id) : id_(id), recorders_(), isplaying_(false) {
}

Player::~Player() {
}

/* WARNING: I felt bad about this. However, it seems I was forced to do this for
 * some reason. Trying hard to remember this reason... by YANG Anran @ 2012.5.9 */
Player::Player(const Player &origin) 
	: id_(origin.id_), recorders_(origin.recorders_), isplaying_(false) {}

/* WARNING: I felt bad about this. However, it seems I was forced to do this for
 * some reason. Trying hard to remember this reason... by YANG Anran @ 2012.5.9 */
Player &Player::operator=(const Player &rhs) {
	this->id_ = rhs.id_;
	this->recorders_ = rhs.recorders_;
	return (*this);
}

int64_t Player::get_id() const {
	return id_;
}

std::weak_ptr<IRecorder> Player::get_recorder(int i) const {
	return recorders_[i];
}

int Player::get_recorders_num() const {
	return recorders_.size();
}

void Player::add_recorder(std::weak_ptr<IRecorder> recorder) {
	recorders_.push_back(recorder);
}

void Player::GoPlay() {
	if (isplaying_) {
		return;
	}
	else {
		isplaying_ = true;
	}

	pthread_t tid;
	/* set detached attr, in order to prevent memory-leak */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, Process, this);
}

void *Player::Process(void *player) {
	Player *subject = static_cast<Player *>(player);
	subject->Play();
	return NULL;
}

std::string Player::get_output() const {
	return "";
}
