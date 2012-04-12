/*
 * =====================================================================================
 *
 *       Filename:  hg_mysql.c
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
#include <stdlib.h>
#include <stdio.h>

#include "hg_mysql.h"

#define DB_SERVER "gdos-yanan"
#define DB_USER "myuser"
#define DB_PASSWORD "mypassword"
#define DB_NAME "higis"
#define DB_PORT 3306

MYSQL_RES *query( char *sql ) {
	int sig;
	MYSQL *conn_ptr;
	MYSQL_RES *res;

	conn_ptr = mysql_init( NULL );

	if ( !conn_ptr ) {
		printf( "MySql init failed\n" );
	}

	conn_ptr = mysql_real_connect( conn_ptr, DB_SERVER, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0 );

	if ( conn_ptr ) {
		printf( "Connection success\n" );
	}
	else {
		printf( "Connection failed\n" );
	}

	sig = mysql_query( conn_ptr, sql );

	if ( sig != 0 ) {
		printf( "Error making query: %s\n", mysql_error( conn_ptr ) );
	}
	else {
		printf( "Query made...\n" );
	}

	res = mysql_store_result( conn_ptr );
	mysql_close( conn_ptr );

	return res;
}
