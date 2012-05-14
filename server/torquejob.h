/*
 * =====================================================================================
 *
 *       Filename:  torquejob.h
 *
 *    Description:  the declaration of job class submitted to Torque PBS.
 *
 *        Version:  1.0
 *        Created:  03/17/2012 03:38:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *                  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _TORQUEJOB_H_
#define _TORQUEJOB_H_

#include <string>

#include "pbsattr.h"
#include "player.h"
#include "config.h"

const float UPDATE_INTERVAL_MS = 500;

/*
 * =====================================================================================
 *        Class:  TorqueJob
 *  Description:  wrapper of the real torque job
 * =====================================================================================
 */
class TorqueJob : public Player
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
        /* constructor, args: process cout; cmdline */
		TorqueJob(int64_t, int, const std::string &);     

		/* ====================  ACCESSORS     ======================================= */
		virtual std::string get_output() const;

		/* ====================  MUTATORS      ======================================= */

		/* ====================  ACTIONS       ======================================= */
		void Play();

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string pbsid_;
		int processcount_;
		std::string cmdline_;
		int connection_;
		std::string scriptpath_;
		std::string outputpath_;
		std::string errlogpath_;
		std::string output_;

		/* ====================  HELPERS       ======================================= */
		std::string GenerateNameByTime() const;
		std::string GenerateScriptName() const;
		const std::string LocWithHost(const std::string &) const;
		/* args: process count */
		const std::string ReqResourceStr(int) const;
		void FillAttr(PbsAttr &);
		char GetStatus() const;

		/* ====================  STEPS         ======================================= */
		/* those who return int: if error, return -1 */
		int CreateScript();
		int Connect();
		int Submit(); /* args: attr */
		int Trace();
		int Collect();
		void Fail();
		void Exit();
}; /* -----  end of class TorqueJob  ----- */

#endif
