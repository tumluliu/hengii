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

Player::Player(int64_t id) : id_(id), bossrecorder_(std::weak_ptr<IRecorder>()), 
	recorders_(), isplaying_(false) {
}

Player::~Player() {
}

int64_t Player::get_id() const {
	return id_;
}

std::weak_ptr<IRecorder> Player::get_recorder(int i) const {
	return recorders_[i];
}

std::weak_ptr<IRecorder> Player::get_bossrecorder() const {
	return bossrecorder_;
}

int Player::get_recorders_num() const {
	return recorders_.size();
}

void Player::add_recorder(std::weak_ptr<IRecorder> recorder) {
	recorders_.push_back(recorder);
}

void Player::set_bossrecorder(std::weak_ptr<IRecorder> bossrecorder) {
	bossrecorder_ = bossrecorder;
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
