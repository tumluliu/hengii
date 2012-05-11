/*
 * =====================================================================================
 *
 *       Filename:  speakingrecorder.h
 *
 *    Description:  Fake class. A class to test the Recorder base class
 *
 *        Version:  1.0
 *        Created:  04/23/2012 02:15:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef  SPEAKINGMOTHER_H_
#define  SPEAKINGMOTHER_H_

#include <stdlib.h>

#include "recorder.h"

/*
 * =====================================================================================
 *        Class:  SpeakingRecorder
 *  Description:  This class print a message in each step as a recorder
 * =====================================================================================
 */
class SpeakingRecorder : public Recorder {
public:
    /* ====================  LIFECYCLE     ======================================= */
    virtual ~SpeakingRecorder();

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

protected:
    /* ====================  EVENTS        ======================================= */
    virtual void AllPlayersDone();



private:
    /* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class SpeakingRecorder  ----- */

#endif
