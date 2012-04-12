/*
 * =====================================================================================
 *
 *       Filename:  hg_pbsattr.c
 *
 *    Description:  This file contains functions to make pbs attrl management easier
 *
 *        Version:  1.0
 *        Created:  04/11/2012 08:26:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "hg_pbsattr.h"

#define ATTR_NAME_LENGTH 256
#define TIME_LENGTH 80

void convertToStdTime( char *result, time_t t ) {
	strftime( result, TIME_LENGTH, "%Y-%m-%d %X", localtime( &t ) );
}

void makeAttrNames( struct attrl *attr ) {
	struct attrl *p = attr;
	if ( p == NULL ) {
		return;
	}

	do {
		p->name = ( char * )malloc( ATTR_NAME_LENGTH + 1 );
		memset( p->name, '\0', ATTR_NAME_LENGTH + 1 );
		p = p->next;
	}
	while ( p != NULL );
}

void freeAttrNames( struct attrl *attr ) {
	struct attrl *p = attr;
	if ( p == NULL ) {
		return;
	}

	do {
		free( p->name );
		p = p->next;
	}
	while ( p != NULL );
}

void getAttrValue( char *result, struct attrl *attr, char *name ) {
	struct attrl *iter = attr;
	char t[TIME_LENGTH];
	bool isFound = false;

	do {
		if ( strcmp( name, iter->name ) == 0 ) {
			isFound = true;
			if (
					strcmp( name, ATTR_ctime ) == 0 ||
					strcmp( name, ATTR_etime ) == 0 ||
					strcmp( name, ATTR_qtime ) == 0 ) {

				convertToStdTime( t, atoi( iter->value ) );
				strcpy( result, t );
			}
			else {
				strcpy( result, iter->value );
			}
		}
		iter = iter->next;
	}
	while ( iter != NULL );

	if ( !isFound ) {
		strcpy( result, "" );
	}
}
