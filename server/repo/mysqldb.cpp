/*
 * =====================================================================================
 *
 *       Filename:  mysqldb.cpp
 *
 *    Description:  some utils only for mysql db
 *
 *        Version:  1.0
 *        Created:  05/04/2012 10:20:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <string>

#include "mysqldb.h"
#include "log.h"
#include <vector>

using std::string;

MySqlDb::MySqlDb() :
	host_(DB_SERVER),username_(DB_USER),
	password_(DB_PASSWORD),dbname_(DB_NAME),port(DB_PORT),
	connpool_(), connlock_() { 
		int i;
		MYSQL *conn;

		for (i = 0; i < MAX_DB_CONN_NUM; i++) {
			if ((conn = CreateConn())) {
				pthread_mutex_t* lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
				pthread_mutex_init(lock, NULL);
				connlock_.push_back(lock);

				connpool_.push_back(conn);
			}
			else {
				break;
			}
		}
	}

MySqlDb::~MySqlDb() {
	for (size_t i = 0; i < connpool_.size(); i++) {
		if ( connpool_[i] ) {
			mysql_close(connpool_[i]);
		}
		pthread_mutex_destroy(connlock_[i]);
		free(connlock_[i]);
	}
}

int MySqlDb::Command( const std::string &command ) {
	int ret;
	int ping;
	MYSQL *conn;

	conn = BorrowConnection();
	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Log().Get(DATABASE, ERROR) << "mysql ping error in command";
		return -1;
	}

	if ( !conn ) {
		Log().Get(DATABASE, ERROR) << "mysql connection lost";
		return -1;
	}

	ret = mysql_query( conn, command.c_str() );
	if ( ret != 0 ) {
		std::string errMsg( mysql_error( conn ) );
		Log().Get(DATABASE, ERROR) << "mysql command error: " << errMsg 
				<< ", sql is: " << command;;
	}
	else {
		//Log().Get(DATABASE, DEBUG) << "mysql command executed successfully. sql is: " << command; // toooo verbose...
	}
	ReturnConnection( conn );

	return 0;
}

MYSQL_RES *MySqlDb::Query( const std::string &query ) const {
	int ret;
	MYSQL *conn;
	MYSQL_RES *res;
	int ping;

	conn = BorrowConnection();

	ping = mysql_ping( conn );
	if ( ping != 0 ) {
		Log().Get(DATABASE, ERROR) << "mysql ping error in query.";
		return NULL;
	}

	ret = mysql_query( conn, query.c_str() );
	res = mysql_store_result( conn );
	if ( ret != 0 ) {
		std::string errMsg( mysql_error( conn ) );
		Log().Get(DATABASE, ERROR) << "mysql query error: " << errMsg
				<< ", sql is: " << query;
	}
	else {
		//Log().Get(DATABASE, DEBUG) << "mysql query executed successfully. sql is: " << query;
	}
	ReturnConnection( conn );

	return res;
}

MYSQL *MySqlDb::BorrowConnection() const {
	unsigned int i;

	do {
		for (i = 0; i < connpool_.size(); i++) {
			//cout << "trying locking " << i << "th conn" << endl; // checking
			if(pthread_mutex_trylock(connlock_[i]) == 0) {
				//cout << i << "th conn locked" << endl;
				return connpool_[i];
			}
		}
	} while(true);
}

int MySqlDb::ReturnConnection(MYSQL *conn) const {
	unsigned int i;
	bool known = false;

	for (i = 0; i < connpool_.size(); i++) {
		if (conn == connpool_[i]) {
			pthread_mutex_unlock(connlock_[i]);
			known = true;
		}
	}

	if (known) {
		return 0;
	}
	else {
		return -1;
	}
}

MYSQL *MySqlDb::CreateConn() const {
	MYSQL *conn = mysql_init( NULL );
	if ( !conn ) {
		Log().Get(DATABASE, ERROR) << "mysql init failed.";
	}

	my_bool reconnect = 1;
	mysql_options( conn, MYSQL_OPT_RECONNECT, &reconnect );

	conn = mysql_real_connect( conn, host_.c_str(), username_.c_str(), password_.c_str(), dbname_.c_str(), port, NULL, 0 );
	if ( conn ) { }
	else {
		Log().Get(DATABASE, ERROR) << "mysql connection failed.";
		return NULL;
	}

	return conn;
}

void MySqlDb::QueryColumn(std::vector<string> &result, const string &sql) const {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;

	res = Query(sql);

	if ( res != NULL ) {
		while (( r = mysql_fetch_row( res ))) {
			if ( r[0] == NULL ) {
				continue;
			}

			result.push_back(string(r[0]));
		}
	}

	mysql_free_result( res );
}

void MySqlDb::QueryRow(std::vector<string> &result, const string &sql) const {
	MYSQL_RES *res;
	MYSQL_ROW r = NULL;

	res = Query(sql);
	if ( res != NULL ) {
		r = mysql_fetch_row( res );
	}

	if ( r != NULL ) {
		for (unsigned int i = 0; i < mysql_num_fields(res); i++) {
			result.push_back(r[i] ? string(r[i]) : "");
		}
	}

	mysql_free_result( res );
}

std::string MySqlDb::QuerySingle(const string &sql) const {
	std::vector<string> res;
	QueryRow(res, sql);
	if (res.size() > 0) {
		return res[0];
	}
	return "";
}
