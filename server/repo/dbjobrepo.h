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
#include <memory>
#include <pthread.h>

#include "jobrepo.h"
#include "flowruntime.h"
#include "jobruntime.h"
#include "sqldb.h"

namespace hpgc { namespace higis { namespace interface { class JobFlow; }}}

/*
 * =====================================================================================
 *        Class:  dbjobrepo
 *  Description:  
 * =====================================================================================
 */
class DbJobRepo : public JobRepo {
	public:
		/* ====================  jobflow LIFECYCLE ================================== */
		DbJobRepo(SqlDb &);
		virtual ~DbJobRepo();

		/* args: job flow runtime info; username */
		virtual int64_t RegisterJobFlow(
				const hpgc::higis::interface::JobFlow &,
				const std::string &);
		virtual std::unique_ptr<FlowRuntime> CreateFlowRuntime(int64_t);
		virtual void UpdateJobFlowRuntime(const FlowRuntime &);
		/* update only flow part of jobflow runtime, not including its jobs' */
		virtual void UpdateFlowPartRuntime(const FlowRuntime &);
		virtual void UpdateJobRuntime(const JobRuntime &);

	protected:
	private:
		/* ====================  DATA MEMBERS  ======================================= */
		SqlDb &db_;
		/* lock the gen-id -> register transaction, to prevent id conflict */
		pthread_mutex_t registerlock_; 
		/* ====================  DISABLED =========================================== */
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
