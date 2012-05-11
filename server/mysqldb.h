/*
 * =====================================================================================
 *
 *       Filename:  mysqldb.h
 *
 *    Description:  some utils only for mysql db
 *
 *        Version:  1.0
 *        Created:  05/04/2012 10:10:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  MYSQLDB_H_
#define  MYSQLDB_H_

#include <vector>
#include <string>
#include <mysql/mysql.h>

#include "config.h"
#include "sqldb.h"

/*
 * =====================================================================================
 *        Class:  MySqlDb
 *  Description:  He offers some mysql utils  
 * =====================================================================================
 */
class MySqlDb : public SqlDb {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		MySqlDb ();                             /* constructor */
		virtual ~MySqlDb();

		/* ====================  DB Operations  ====================================== */
		int Command(const std::string&);
		/* args: 1:sql, 2:result list to fill */
		void QueryRow(std::vector<std::string> &, const std::string &);
		void QueryColumn(std::vector<std::string> &, const std::string &);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string host_;
		std::string username_;
		std::string password_;
		std::string dbname_;
		int port;

		/* ====================  Connection pool  ==================================== */
		std::vector<MYSQL*> connpool_;
		std::vector<pthread_mutex_t*> connlock_;
		MYSQL* BorrowConnection();
		int ReturnConnection(MYSQL *);

		/* ====================  DB Operations  ====================================== */
		MYSQL *CreateConn();
		MYSQL_RES* Query( const std::string & );
}; /* -----  end of class MySqlDb  ----- */


#endif
