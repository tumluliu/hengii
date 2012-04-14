/*
 * =====================================================================================
 *
 *       Filename: hg_monitor.c
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
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <pbs_error.h>
#include <pbs_ifl.h>

#include "hg_pbsattr.h"
#include "hg_mysql.h"
#include "hg_utils.h"

#define MAX_PBS_ID_LENGTH 256
#define HOSTNAME_SIZE 1024
#define HOST_DOMAIN ".localhost.localdomain"
#define PBS_ATTR_COUNT 16
#define MAX_PBS_ATTR_VALUE_LENGTH 256
#define MAX_PBS_RESOURCE_LENGTH 1024
#define MAX_SQL_LENGTH 4096
#define INTERVAL_MILLISECONDS 500


/* 
 * ===  FUNCTION  ======================================================================
 *  	   Name:  writeJobStatus
 *  Description:  write job status into database, return the status flag:
 *                (interpreted on the thrift interface)
 *                FINISHED = 1,
 *                RUNNING = 2,
 *                WAITING_FOR_SUBMIT = 3,
 *                QUEUING = 4,
 *                FAILED = 5,
 *                NOT_EXIST = 6,
 *                PAUSED = 7
 * =====================================================================================
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  writePbsJobStatus
 *  Description:  write pbs job status into database, return pbs status
 * =====================================================================================
 */
void writePbsJobStatus( char *id, struct attrl *attrRuntime, MYSQL *conn ) {
	char sql[MAX_SQL_LENGTH];
	struct attrl *iter;
	char resourceList[MAX_PBS_RESOURCE_LENGTH];
	char resourceUsed[MAX_PBS_RESOURCE_LENGTH];
	char buffer[MAX_PBS_ATTR_VALUE_LENGTH];

	memset( buffer, '\0', MAX_PBS_ATTR_VALUE_LENGTH );
	memset( sql, '\0', MAX_SQL_LENGTH );
	memset( resourceList, '\0', MAX_PBS_RESOURCE_LENGTH );
	memset( resourceUsed, '\0', MAX_PBS_RESOURCE_LENGTH );

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

	strcpy( sql, "update gdos_sys_pbsjob " );
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
	strcat( sql, "' where id='" );
	strcat( sql, id );
	strcat( sql, "';" );

	printf("The sql is: %s\n", sql);
	command( sql, conn );
}

void updatePbsStatus( char *id, int pbsConn, MYSQL *conn ) {
	struct attrl attr[PBS_ATTR_COUNT];
	int i;
	struct batch_status *stat;
	struct attrl *attrRuntime;

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

	printf("query id is: %s, batch status name is: %s, text is: %s, next is: %d\n", id, stat->name, stat->text, stat->next );

	attrRuntime = stat->attribs;
	if ( attrRuntime == NULL ) {
		return;
	}

	writePbsJobStatus(id, attrRuntime, conn);

	pbs_statfree( stat );
}


MYSQL_RES *getPbsJobIds( int *count, MYSQL *conn ) {
	MYSQL_RES *res;
	char *sql = "select id from gdos_sys_pbsjob where job_state <> 'C' or job_state is null;";

	res = query( sql, conn );
	*count = (res != NULL) ? mysql_num_rows( res ) : 0;

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
		if (row[0] == NULL) {
			continue;
		}
		strcpy( ids[i], row[0] );
		i++;
	}
}

int main( int argc, char **argv ) {
	char **ids;
	int idCount;
	int i;
	MYSQL_RES *res;
	MYSQL *dbConn;
	int pbsConn;
	char hostname[HOSTNAME_SIZE];

	gethostname( hostname, sizeof( hostname ) );
	dbConn = mySqlConnect();
	if (dbConn == NULL) {
		fprintf( stderr, "mysql connection failed\n" );
		return -1;
	}
	pbsConn = pbs_connect( hostname );

	//warning: connection test!!!!
	printf( "pbs Connection=%d\n", pbsConn );

	do {
		res = getPbsJobIds( &idCount, dbConn );
		ids = makeStringArray( idCount, MAX_PBS_ID_LENGTH );
		fillPbsJobIds( res, ids );
		for ( i = 0; i < idCount; i++ ) {
			updatePbsStatus( ids[i], pbsConn, dbConn );
		}

		mysql_free_result(res);
		freeStringArray( ids, idCount );

		usleep(INTERVAL_MILLISECONDS);
	} while(1);

	pbs_disconnect( pbsConn );

	mySqlClose(dbConn);

	return 0;
}
