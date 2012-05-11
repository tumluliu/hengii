/*
 * =====================================================================================
 *
 *       Filename:  dbjobrepo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/08/2012 03:13:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  WU Jiani, LIU Lu, YANG Anran
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  DBJOBREPO_H_
#define  DbJOBREPO_H_

#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <vector>
#include <pthread.h>

#include "hpgcjob_types.h"
#include "jobrepo.h"
#include "flowruntime.h"
#include "jobruntime.h"
#include "sqldb.h"

const std::string PBS_JOB_TABLE_NAME = "gdos_sys_pbsjob";
const std::string JOB_TABLE_NAME = "gdos_sys_job";
const std::string JOB_FLOW_TABLE_NAME = "gdos_sys_jobflow";

/* forward declaration, because he must be dbjobrepo's friend */
class JobRepoEntry;
/*
 * =====================================================================================
 *        Class:  dbjobrepo
 *  Description:  
 * =====================================================================================
 */
class DbJobRepo : public JobRepo {
	public:
		/* ====================  jobflow LIFECYCLE ================================== */
		/* args: job flow runtime info; username */
		virtual int64_t RegisterJobFlow(
				const hpgc::higis::interface::JobFlow &,
				const std::string &);
		virtual FlowRuntime *BorrowJobFlowRuntime(int64_t);
		virtual void ReturnJobFlowRuntime(FlowRuntime*);
		virtual void UpdateJobFlowRuntime(const FlowRuntime *);
		/* update only flow part of jobflow runtime, not including its jobs' */
		virtual void UpdateFlowPartRuntime(const FlowRuntime *);
		virtual void UpdateJobRuntime(const JobRuntime *);

		/* ====================  friend, the only one to create self ================= */
		friend class JobRepoEntry;
	protected:
	private:
		/* ====================  DATA MEMBERS  ======================================= */
		SqlDb *db_;
		/* lock the gen-id -> register transaction, to prevent id conflict */
		pthread_mutex_t registerlock_; 
		/* ====================  LIFECYCLE(hidden because of singleton)  ============= */
		DbJobRepo(SqlDb *);
		virtual ~DbJobRepo();
		DbJobRepo(DbJobRepo const&);
		DbJobRepo &operator=(DbJobRepo const&);

		/* ====================  SQLS          ======================================= */
		const std::string RegisterJobSql(int64_t, int, const std::string&) const;
		const std::string RegisterJobFlowSql(int64_t, const std::string&) const;
		const std::string UpdateJobStatusSql(int64_t, int, int, const std::string&) const;
		const std::string GetFlowStatusSql( int64_t ) const;
		const std::string GetJobStatusSql( int64_t, int ) const;
		const std::string GetJobCountSql( int64_t flowId ) const;
		const std::string UpdateJobFlowStatusSql(int64_t, int, const std::string&) const;
		const std::string GetJobIdsSql(int64_t) const;
		const std::string GetConflictFlowIdSql(int64_t) const;

		/* ====================  MUTATORS      ======================================= */
		/* args: job flow id; job id; username */
		int RegisterJobToDb(int64_t, int, const std::string&);
		/* args: job flow id; username */
		int RegisterJobFlowToDb(int64_t, const std::string&);
		/* args: job flow id; job id; status; message */
		int UpdateJobStatus(int64_t, int, int, const std::string&);
		/* args: job flow id; status; message */
		int UpdateJobFlowStatus(int64_t, int, const std::string&);

		/* ====================  ACCESSORS     ======================================= */
		int GetJobCount( int64_t flowId );
		int GetFlowStatus( int64_t, std::string& );
		int GetJobStatus( int64_t, int, std::string& );
		bool HasConflictFlowId(int64_t) const;

		/* ====================  Utils         ======================================= */
		std::string GetCurrentTime();
		int64_t GenerateFlowId();
		void GetJobIds( int64_t, std::vector<int> & );
		void EscapeSingleQuote( std::string &origin ) const; 
}; /* -----  end of class Joblog  ----- */

#endif
