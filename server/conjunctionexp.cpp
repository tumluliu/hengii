/*
 * =====================================================================================
 *
 *       Filename:  conjunctionExp.cpp
 *
 *    Description:  The class of conjunction expression.
 *
 *        Version:  1.0
 *        Created:  04/23/2012 09:11:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "conjunctionexp.h"

ConjunctionExp::ConjunctionExp(int size) : content_(size), lock_() {
    int i;

    for (i = 0; i < size; i++) {
        content_[i] = false;
    }

    pthread_mutex_init(&lock_, NULL);
}

ConjunctionExp::~ConjunctionExp() {
    pthread_mutex_destroy(&lock_);
}

bool ConjunctionExp::Set(int pos, bool value) {
    bool result = false;
    size_t i;

    pthread_mutex_lock(&lock_);
    content_[pos] = value;
    result = true;
    for (i = 0; i < content_.size(); i++) {
        if (!content_[i]) {
            result = false;
            break;
        }
    }
    pthread_mutex_unlock(&lock_);

    return result;
}

void ConjunctionExp::Concat(const ConjunctionExp &tail) {
    pthread_mutex_lock(&lock_);
    content_.insert(content_.end(), tail.content_.begin(), tail.content_.end());
    pthread_mutex_unlock(&lock_);
}

void ConjunctionExp::Clear() {
    pthread_mutex_lock(&lock_);
	content_.clear();
    pthread_mutex_unlock(&lock_);
}
