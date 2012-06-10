/*
 * =====================================================================================
 *
 *       Filename:  irecorder.h
 *
 *    Description:  The pure interface of recorder
 *
 *        Version:  1.0
 *        Created:  04/23/2012 07:42:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef  IMOTHER_H_
#define  IMOTHER_H_

#include <vector>
#include <stdint.h>

class IRecorder {
public:
    /* ====================  LIFECYCLE     ======================================= */
    virtual ~IRecorder();

    /* ====================  EVENTS        ======================================= */
    /* input: 1:the done player id */
    virtual void OnePlayerDone(int64_t) = 0;
    virtual void OnePlayerCanceled(int64_t) = 0;
};

#endif
