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

MYSQL *mySqlConnect() {
	MYSQL *conn = mysql_init( NULL );

	if ( !conn ) {
		fprintf( stderr, "MySql init failed\n" );
		return NULL;
	}

	conn = mysql_real_connect( conn, DB_SERVER, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0 );

	if ( conn ) {
		fprintf( stderr, "Connection success\n" );
	}
	else {
		fprintf( stderr, "Connection failed\n" );
		return NULL;
	}

	return conn;
}

void mySqlClose(MYSQL *conn) {
	mysql_close(conn);
}

MYSQL_RES *query( char *sql, MYSQL *conn ) {
	int sig;
	MYSQL_RES *res;

	sig = mysql_query( conn, sql );

	if ( sig != 0 ) {
		fprintf( stderr, "Error making query: %s\n", mysql_error( conn ) );
	}
	else {
		fprintf( stderr, "Query made...\n" );
	}

	res = mysql_store_result( conn );

	return res;
}

void command( char *sql, MYSQL *conn ) {
	int sig;

	sig = mysql_query( conn, sql );

	if ( sig != 0 ) {
		fprintf( stderr, "Error making command: %s\n", mysql_error( conn ) );
	}
	else {
		fprintf( stderr, "Command made...\n" );
	}
}
