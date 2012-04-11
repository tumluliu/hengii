/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/11/2012 10:12:01 AM
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
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <pbs_error.h>
#include <pbs_ifl.h>

#define MAX_PBS_ID_LENGTH 256
#define ATTR_NAME_LENGTH 256
#define TIME_LENGTH 80
#define HOSTNAME_SIZE 1024
#define PBS_ATTR_COUNT 16
#define MAX_PBS_ATTR_VALUE_LENGTH 256
#define MAX_PBS_RESOURCE_LENGTH 1024
#define MAX_SQL_LENGTH 4096
#define HOST_DOMAIN ".localhost.localdomain"
#define DB_SERVER "gdos-yanan"
#define DB_USER "myuser"
#define DB_PASSWORD "mypassword"
#define DB_NAME "higis"
#define PORT 3306
#define INTERVAL_MICRO_SECONDS 100

void convertToMysqlTime( char *result, time_t t ) {
	strftime( result, TIME_LENGTH, "%Y-%m-%d %X", localtime( &t ) );
}

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

MYSQL_RES *query( char *sql ) {
	int sig;
	MYSQL *conn_ptr;
	MYSQL_RES *res;

	conn_ptr = mysql_init( NULL );

	if ( !conn_ptr ) {
		printf( "MySql init failed\n" );
	}

	conn_ptr = mysql_real_connect( conn_ptr, DB_SERVER, DB_USER, DB_PASSWORD, DB_NAME, PORT, NULL, 0 );

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

				convertToMysqlTime( t, atoi( iter->value ) );
				printf( "time is: %s\n", t );
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

void updateStatus( char *id, int pbsConn ) {
	struct attrl attr[PBS_ATTR_COUNT];
	int i;
	struct batch_status *stat;
	char sql[MAX_SQL_LENGTH];
	struct attrl *attrRuntime;
	struct attrl *iter;
	char resourceList[MAX_PBS_RESOURCE_LENGTH];
	char resourceUsed[MAX_PBS_RESOURCE_LENGTH];
	char buffer[MAX_PBS_ATTR_VALUE_LENGTH];

	memset( buffer, '\0', MAX_PBS_ATTR_VALUE_LENGTH );
	memset( sql, '\0', MAX_SQL_LENGTH );
	memset( resourceList, '\0', MAX_PBS_RESOURCE_LENGTH );
	memset( resourceUsed, '\0', MAX_PBS_RESOURCE_LENGTH );

	for ( i = 0; i < PBS_ATTR_COUNT; i++ ) {
		attr[i].value = ( char * )NULL;
		attr[i].resource = ( char * )NULL;
		attr[i].next = i == PBS_ATTR_COUNT - 1 ? ( struct attrl * )NULL : &attr[i + 1];
	}

	makeAttrNames( attr );
	strcpy( attr[0].name, ATTR_N );
	strcpy( attr[1].name, ATTR_ctime );
	strcpy( attr[2].name, ATTR_etime );
	strcpy( attr[3].name, ATTR_e );
	strcpy( attr[4].name, ATTR_exechost );
	strcpy( attr[5].name, ATTR_euser );
	strcpy( attr[6].name, ATTR_j );
	strcpy( attr[7].name, ATTR_o );
	strcpy( attr[8].name, ATTR_owner );
	strcpy( attr[9].name, ATTR_queue );
	strcpy( attr[10].name, ATTR_qtime );
	strcpy( attr[11].name, ATTR_state );
	strcpy( attr[12].name, ATTR_server );
	strcpy( attr[13].name, ATTR_v );
	strcpy( attr[14].name, ATTR_used );
	strcpy( attr[15].name, ATTR_l );

	stat = pbs_statjob( pbsConn, id, attr, 0 );

	freeAttrNames( attr );

	if ( stat == NULL ) {
		return;
	}

	attrRuntime = stat->attribs;
	if ( attrRuntime == NULL ) {
		return;
	}

	/* init resource list, which is an xml string */
	strcpy( resourceList, "<xml>" );
	strcpy( resourceUsed, "<xml>" );

	iter = attrRuntime;
	do {
		if ( strcmp( iter->name, ATTR_l ) == 0 ) {
			sprintf( resourceList, "%s<a v=\"%s\">%s</a>", resourceList, iter->resource, iter->value ? iter->value : "" );
		}
		else if ( strcmp( iter->name, ATTR_used ) == 0 ) {
			sprintf( resourceUsed, "%s<a v=\"%s\">%s</a>", resourceUsed, iter->resource, iter->value ? iter->value : "" );
		}
		iter = iter->next;
	}
	while ( iter != NULL );

	/* complete resource list */
	strcat( resourceList, "</xml>" );
	strcat( resourceUsed, "</xml>" );

	strcpy( sql, "update gdos_sys_job " );
	strcat( sql, "set job_name='" );
	getAttrValue( buffer, attrRuntime, ATTR_N );
	strcat( sql, buffer );
	strcat( sql, "',ctime='" );
	getAttrValue( buffer, attrRuntime, ATTR_ctime );
	strcat( sql, buffer );
	strcat( sql, "',execution_time='" );
	getAttrValue( buffer, attrRuntime, ATTR_etime );
	strcat( sql, buffer );
	strcat( sql, "',exec_host='" );
	getAttrValue( buffer, attrRuntime, ATTR_exechost );
	strcat( sql, buffer );
	strcat( sql, "',euser='" );
	getAttrValue( buffer, attrRuntime, ATTR_euser );
	strcat( sql, buffer );
	strcat( sql, "',job_owner='" );
	getAttrValue( buffer, attrRuntime, ATTR_owner );
	strcat( sql, buffer );
	strcat( sql, "',queue='" );
	getAttrValue( buffer, attrRuntime, ATTR_queue );
	strcat( sql, buffer );
	strcat( sql, "',qtime='" );
	getAttrValue( buffer, attrRuntime, ATTR_qtime );
	strcat( sql, buffer );
	strcat( sql, "',job_state='" );
	getAttrValue( buffer, attrRuntime, ATTR_state );
	strcat( sql, buffer );
	strcat( sql, "',server='" );
	getAttrValue( buffer, attrRuntime, ATTR_server );
	strcat( sql, buffer );
	strcat( sql, "',variable_list='" );
	getAttrValue( buffer, attrRuntime, ATTR_v );
	strcat( sql, buffer );
	strcat( sql, "',resource_list='" );
	strcat( sql, resourceList );
	strcat( sql, "',resource_used='" );
	strcat( sql, resourceUsed );
	strcat( sql, "' where pbs_job_id='" );
	strcat( sql, id );
	strcat( sql, "';" );

	printf( "the sql for updating mysql is: %s\n", sql );

	pbs_statfree( stat );

	mysql_free_result(query( sql ));
}

void freeRes( MYSQL_RES *res ) {
	mysql_free_result( res );
}

MYSQL_RES *getPbsJobIds( int *count ) {
	MYSQL_RES *res;
	char *sql = "select pbs_job_id from gdos_sys_job;";

	res = query( sql );
	*count = mysql_num_rows( res );

	return res;
}

void fillPbsJobIds( MYSQL_RES *res, char **ids ) {
	MYSQL_ROW row;
	unsigned int i;

	if ( res == NULL ) {
		return;
	}

	i = 0;
	while ( ( row = mysql_fetch_row( res ) ) ) {
		strcpy( ids[i], row[0] );
		i++;
	}
}

int main( int argc, char **argv ) {
	/*test2();*/
	/*return 0;*/
	char **ids;
	int idCount;
	int i;
	MYSQL_RES *res;
	int pbsConn;
	char hostname[HOSTNAME_SIZE];

	gethostname( hostname, sizeof( hostname ) );
	pbsConn = pbs_connect( hostname );
	//warning: connection test!!!!
	printf( "pbs Connection=%d\n", pbsConn );

	do {
		res = getPbsJobIds( &idCount );
		ids = makeStringArray( idCount, MAX_PBS_ID_LENGTH );
		fillPbsJobIds( res, ids );
		for ( i = 0; i < idCount; i++ ) {
			updateStatus( ids[i], pbsConn );
		}

		mysql_free_result(res);
		freeStringArray( ids, idCount );

		usleep(INTERVAL_MICRO_SECONDS);
	} while(1);

	pbs_disconnect( pbsConn );

	return 0;
}
