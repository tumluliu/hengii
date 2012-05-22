/*
 * =====================================================================================
 *
 *       Filename:  dbdatarepo.h
 *
 *    Description:  data repo with sql db
 *
 *        Version:  1.0
 *        Created:  05/18/2012 03:10:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  DBDATAREPO_H_
#define  DBDATAREPO_H_

#include <pthread.h>

#include "datarepo.h"
#include "sqldb.h"

/*
 * =====================================================================================
 *        Class:  DbDataRepo
 *  Description:  He offers data repo's function with a sql db
 * =====================================================================================
 */
class DbDataRepo : public DataRepo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		DbDataRepo(SqlDb &); /* constructor */
		virtual ~DbDataRepo();

		/* ====================  TOOLS         ======================================= */
		/* This is a fast-die lock, i.e. if not successful, it will immediately return 
		 * a non 0 value to indicate failure, otherwise it will lock the data
		 * and return 0. */
		/* args: (out)fail message;the id of requesting flow instance;requests */
		virtual int Lock(std::string &, int64_t, const std::vector<DataRequest> &);
		/* args: the id of requesting flow instance;requests */
		virtual void UnLock(int64_t);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		SqlDb &db_;
		pthread_mutex_t lock_;

		/* ====================  HELPERS       ======================================= */
		/* the following 3: args: uri of data;data type;usage(use int to prevent 
		 * dependency here). Forward declaration doesn't work here because it contains
		 * enum. */
		bool Exist(const std::string &, int) const;
		bool IsUsedFor(const std::string &, int, int) const;
		bool IsUsed(const std::string &, int) const;
		/* args: user;uri of data;data type;access type(use int to prevent dependency here)*/
		void WriteUsage(int64_t, const std::string &, int, int);
		void RemoveUsage(int64_t);

		/* ====================  STEPS         ======================================= */
		/* args: (out)fail message;requests */
		int Check(std::string &, const std::vector<DataRequest> &) const;
}; /* -----  end of class DbDataRepo  ----- */

#endif
