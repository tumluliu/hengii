/*
 * =====================================================================================
 *
 *       Filename:  jobruntime.h
 *
 *    Description:  class contains runtime informantion of a job
 *
 *        Version:  1.0
 *        Created:  05/02/2012 08:38:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  JOBRUNTIME_H_
#define  JOBRUNTIME_H_

#include <string>

#include "status.h"

/*
 * =====================================================================================
 *        Class:  JobRuntime
 *  Description:  The runtime informantion of a job, containing full infos for admin,
 *                not user 
 * =====================================================================================
 */
class JobRuntime
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		JobRuntime(int64_t, int);                 /* constructor arg: id, flow id */
		JobRuntime(const JobRuntime&);            /* copy constructor */

		/* ====================  ACCESSORS     ======================================= */
		int64_t get_fid() const;
		int get_id() const;
		JobStatus::type get_status() const;
		std::string get_message() const;

		/* ====================  MUTATORS      ======================================= */
		void set_status(const JobStatus::type &);
		void set_message(const std::string &);

		/* ====================  OPERATORS     ======================================= */
		JobRuntime &operator=(const JobRuntime &);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		int id_;
		int64_t fid_;
		JobStatus::type status_;
		std::string message_;

		/* ====================  Utils         ======================================= */
		void Copy(const JobRuntime &);

}; /* -----  end of class Jobruntime  ----- */

#endif
