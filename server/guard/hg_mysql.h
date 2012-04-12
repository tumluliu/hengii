/*
 * =====================================================================================
 *
 *       Filename:  hg_mysql.h
 *
 *    Description:  This file contains some mysql utility functions
 *
 *        Version:  1.0
 *        Created:  04/11/2012 08:44:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <mysql/mysql.h>

#ifndef MONITOR_HG_MYSQL_H_
#define MONITOR_HG_MYSQL_H_

MYSQL_RES *query( char *sql ); 

#endif
