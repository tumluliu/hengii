/*
 * =====================================================================================
 *
 *       Filename:  flowruntime.h
 *
 *    Description:  class contains runtime informantion of a job flow
 *
 *        Version:  1.0
 *        Created:  05/02/2012 11:09:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  FLOWRUNTIME_H_
#define  FLOWRUNTIME_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "jobruntime.h"
#include "status.h"

/*
 * =====================================================================================
 *        Class:  FlowRuntime
 *  Description:  The runtime informantion of a job flow, containing full infos for admin,
 *                not user 
 * =====================================================================================
 */
class FlowRuntime
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		FlowRuntime(int64_t);                             /* constructor, arg: id */
		FlowRuntime(const FlowRuntime &);

		/* ====================  ACCESSORS     ======================================= */
		int64_t get_id() const;
		Status::type get_status() const;
		std::string get_message() const;
		int get_jobcount() const;
		std::shared_ptr<JobRuntime> get_jobruntime(int) const;
		std::shared_ptr<JobRuntime> get_jobruntime_byid(int) const;

		/* ====================  MUTATORS      ======================================= */
		void set_status(const Status::type &);
		void set_message(const std::string &);
		void update_jobruntime(const JobRuntime &);
		void add_jobruntime(std::shared_ptr<JobRuntime>);

		/* ====================  OPERATORS     ======================================= */
		FlowRuntime &operator=(const FlowRuntime &);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		int64_t id_;
		Status::type status_;
		std::string message_;
		std::vector<std::shared_ptr<JobRuntime> > jobdetails_;

		/* ====================  Utils         ======================================= */
		void Copy(const FlowRuntime &);

}; /* -----  end of class FlowRuntime  ----- */

#endif
