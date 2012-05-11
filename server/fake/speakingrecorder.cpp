/*
 * =====================================================================================
 *
 *       Filename:  speakingrecorder.cpp
 *
 *    Description:  Fake class. A class to test the Recorder base class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 02:20:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <iostream>

#include "speakingrecorder.h"

SpeakingRecorder::~SpeakingRecorder() {
}

void SpeakingRecorder::AllPlayersDone() {
    std::cout << "all done!" << std::endl;
}
