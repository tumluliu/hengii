/*
 * =====================================================================================
 *
 *       Filename:  conjunctionExp.h
 *
 *    Description:  The class of conjunction expression.
 *
 *        Version:  1.0
 *        Created:  04/23/2012 09:00:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef  CONJUNCTIONEXP_H_
#define  CONJUNCTIONEXP_H_

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <stdbool.h>
#include <vector>
#include <pthread.h>

/*
 * =====================================================================================
 *        Class:  ConjunctionExp
 *  Description:  It consists of several truth-values. When set a value at a position, it
 *                will return the value of entire expression. This operation is atomic,
 *                i.e. the thread-safety can be guranteed.
 * =====================================================================================
 */
class ConjunctionExp {
public:
    /* ====================  LIFECYCLE     ======================================= */
    ConjunctionExp(int);                                /* constructor */
    ~ConjunctionExp();                             /* destructor  */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* Set truth-value at given position and Return the value of
     * the entire expression. Atomic.
     * input:
     *   1:position
     *   2:value
     * output:
     *   the value of entire expression */
    bool Set(int, bool);
    /* Add another conjunction expression to the tail */
    void Concat(const ConjunctionExp &);
	void Clear();

    /* ====================  OPERATORS     ======================================= */

protected:
    /* ====================  DATA MEMBERS  ======================================= */

private:
    /* ====================  DATA MEMBERS  ======================================= */
    /* The values of each element in the expression */
    std::vector<bool> content_;
    /* ====================  ASSISTANTS    ======================================= */
    /* to make the set operation atomic */
    pthread_mutex_t lock_;
}; /* -----  end of class ConjunctionExp  ----- */

#endif
