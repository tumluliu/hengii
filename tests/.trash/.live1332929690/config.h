/*
 * =====================================================================================
 *
 *       Filename:  config.h.in
 *
 *    Description:  The config variables
 *
 *        Version:  0.6
 *        Created:  03/19/2012 04:29:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string>

#ifndef _CONFIG_H_
#define _CONFIG_H_

using namespace std;

const string MPI_CONF_PATH    = "/home/geohpc/higine/tests/live/mpienv.conf";
const string RUN_LOG_FILE     = "/home/geohpc/higine/tests/live/run.log";
const string PBS_OUT_DIR      = "/home/geohpc/higine/tests/live/pbstmp/";
const string APP_DIR          = "/home/geohpc/higine/tests/live/apps/";

#endif
