/*
 * =====================================================================================
 *
 *       Filename:  jobrepoentry.cpp
 *
 *    Description:  the only entry of job repo
 *
 *        Version:  1.0
 *        Created:  05/03/2012 09:10:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "jobrepoentry.h"
#include "dbjobrepo.h"
#include "sqldb.h"
#include "mysqldb.h"

JobRepo *JobRepoEntry::m_instance = NULL;

JobRepo *JobRepoEntry::Open() {
	if (m_instance == NULL) {
		SqlDb *db = new MySqlDb;
		m_instance = new DbJobRepo(db);
	}
	return m_instance;
}

void JobRepoEntry::Close() {
	// WARNING: not very clean but rather directly.
	// by YANG Anran @2012.5.9
	delete ((dynamic_cast<DbJobRepo*>(m_instance))->db_);
	delete m_instance;
}
