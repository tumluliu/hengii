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

#define DB_SERVER "127.0.0.1"
#define DB_USER "myuser"
#define DB_PASSWORD "mypassword"
#define DB_NAME "higis"
#define DB_PORT 3306

MYSQL *mySqlConnect();
void mySqlClose(MYSQL *conn);
MYSQL_RES *query( char *sql, MYSQL *conn ); 
void command( char *sql, MYSQL *conn ); 

#endif
