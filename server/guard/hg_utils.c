/*
 * =====================================================================================
 *
 *       Filename:  hg_utils.c
 *
 *    Description:  some useful tools (I'm not dustbin!!! I'm not trashhhhhhhh!!!)
 *
 *        Version:  1.0
 *        Created:  04/11/2012 08:54:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>

char **makeStringArray( int count, int stringSize ) {
	char **result;
	int i;

	result = ( char ** )calloc( count, sizeof( char * ) );
	for ( i = 0; i < count; i++ ) {
		result[i] = ( char * )calloc( stringSize, sizeof( char ) );
		memset( result[i], '\0', stringSize );
	}

	return result;
}

void freeStringArray( char **a, int count ) {
	int i;
	for ( i = 0; i < count; i++ ) {
		free( a[i] );
	}
	free( a );
}
