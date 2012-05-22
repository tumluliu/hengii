/*
 * =====================================================================================
 *
 *       Filename:  dbdatarepo.cpp
 *
 *    Description:  data repo with sql db
 *
 *        Version:  1.0
 *        Created:  05/18/2012 04:47:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "dbdatarepo.h"
#include "datarequest.h"
#include "config.h"
#include "log.h"

using std::string;

DbDataRepo::DbDataRepo(SqlDb &db) : db_(db), lock_() {
	pthread_mutex_init(&lock_, NULL);
}

DbDataRepo::~DbDataRepo() {
	pthread_mutex_destroy(&lock_);
}

int DbDataRepo::Lock(std::string &out_msg, int64_t id, 
		const std::vector<DataRequest> &reqlist) { 
	int res;
	pthread_mutex_lock(&lock_);
	if ((res = Check(out_msg, reqlist)) == 0) {
		/* persist */
		for (auto i = reqlist.begin(); i != reqlist.end(); i++) {
			WriteUsage(id, i->get_datauri(), i->get_datatype(), i->get_accesstype());
		}
	}
	pthread_mutex_unlock(&lock_);
	return res;
}

int DbDataRepo::Check(std::string &out_msg,
		const std::vector<DataRequest> &reqlist) const { 
	for (auto i = reqlist.begin(); i != reqlist.end(); i++) {
		if (i->get_accesstype() == Access::CREATE) {
			if (Exist(i->get_datauri(), i->get_datatype())) {
				out_msg = "Create data " + i->get_datauri() 
					+ " failure for it already exists!";
				return -1;
			}
		}
		else if (i->get_accesstype() == Access::UPDATE) {
			if (IsUsed(i->get_datauri(), i->get_datatype())) {
				out_msg = "Cannot update data " + i->get_datauri()
					+ " for it is being used by others!";
				return -1;
			}
		}
		else if (i->get_accesstype() == Access::READ) {
			if (IsUsedFor(i->get_datauri(), i->get_datatype(), Access::UPDATE)
					|| IsUsedFor(i->get_datauri(), i->get_datatype(), 
						Access::CREATE)) {
				out_msg = "Cannot read data " + i->get_datauri()
					+ " for it is being written by others!";
				return -1;
			}
		}
		else {}
	}
	return 0;
}

void DbDataRepo::UnLock(int64_t id) { 
	pthread_mutex_lock(&lock_);
	RemoveUsage(id);
	pthread_mutex_unlock(&lock_);
}

bool DbDataRepo::Exist(const std::string &datauri, int datatype) const {
	/* WARNING: ugly code here because several databases resemble but not the same.
	 * It feels like the database I met before our app-data model. History is back
	 * ... by YANG Anran @ 2012.5.19 */
	string reftable;
	if (datatype == DataType::FEATURE) {
		reftable = FEATURE_TABLE_NAME;
	}
	else if (datatype == DataType::RASTER) {
		reftable = RASTER_TABLE_NAME;
	}
	std::stringstream sql;
	std::stringstream sql2;
	/* already exists */
	sql << "select count(*) from " << reftable
		<< " where name = '" << datauri << "';";
	/* some one is creating it */
	sql2 << "select count(*) from " << DATA_STAT_TABLE_NAME
		<< " where name = '" << datauri << "' and datatype = " << datatype
		<< " and datausage = " << Access::CREATE << ";";

	if (db_.QuerySingle(sql.str()) == "0" && db_.QuerySingle(sql.str()) == "0") {
		return false;
	}
	else {
		return true;
	}
}

bool DbDataRepo::IsUsedFor(const std::string &datauri, int datatype, int usage) const {
	std::stringstream sql;
	sql << "select count(*) from " << DATA_STAT_TABLE_NAME
		<< " where name = '" << datauri << "' and datatype = " << datatype
		<< " and datausage = " << usage << ";";
	if (db_.QuerySingle(sql.str()) == "0") {
		return false;
	}
	else {
		return true;
	}
}

bool DbDataRepo::IsUsed(const std::string &datauri, int datatype) const {
	std::stringstream sql;
	sql << "select count(*) from " << DATA_STAT_TABLE_NAME
		<< " where name = '" << datauri << "' and datatype = " << datatype << ";";
	if (db_.QuerySingle(sql.str()) == "0") {
		return false;
	}
	else {
		return true;
	}
}

void DbDataRepo::WriteUsage(int64_t user, const std::string &datauri, 
		int datatype, int usage) {
	std::stringstream sql;
	sql << "insert into " << DATA_STAT_TABLE_NAME
		<< "(flowid,name,datatype,datausage) values('"
		<< user << "', '" << datauri << "', '"
		<< datatype << "', '" << usage << "');";
	db_.Command(sql.str());
}

void DbDataRepo::RemoveUsage(int64_t user) {
	std::stringstream sql;
	sql << "delete from " << DATA_STAT_TABLE_NAME
		<< " where flowid = " << user << ";";
	db_.Command(sql.str());
}
