/*
 * =====================================================================================
 *
 *       Filename:  sqldb.h
 *
 *    Description:  abstract relational database
 *
 *        Version:  1.0
 *        Created:  05/04/2012 11:53:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  SQLDB_H_
#define  SQLDB_H_

#include <string>
#include <vector>

/*
 * =====================================================================================
 *        Class:  SqlDb
 *  Description:  He offers some relational database functions
 * =====================================================================================
 */
//!!!!!!!!!!!not implement yet for the dbjobrepo still contains mysql staff!!!!!!!!!!!!
class SqlDb {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		virtual ~SqlDb();

		/* ====================  DB Operations  ====================================== */
		virtual int Command(const std::string&) = 0;
		/* args: 1:sql, 2:result list to fill */
        virtual void QueryRow(std::vector<std::string> &, const std::string &) = 0;
        virtual void QueryColumn(std::vector<std::string> &, const std::string &) = 0;

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class SqlDb  ----- */

#endif
