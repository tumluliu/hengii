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
#include "resources.h"

struct ExitReason {
	enum type {
		NORMAL = 1,
		CANCELED = 2,
		FAILED = 3
	};
};

struct BatchSignal {
	enum type {
		CONTINUE = 1,
		CANCEL = 2,
		PAUSE = 3,
		RESUME = 4,
		FAIL = 5
	};
};

const float UPDATE_INTERVAL_MS = 500;
const int WAIT_FOR_OUT_S = 1;

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
		TorqueJob(int64_t, const Resources &, const std::string &);     

		/* ====================  ACCESSORS     ======================================= */
		virtual std::string get_output() const;

		/* ====================  MUTATORS      ======================================= */

		/* ====================  ACTIONS       ======================================= */
		virtual void Stop();


	protected:
		/* ====================  ACTIONS       ======================================= */
		virtual void Play();

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string pbsid_;
		Resources resources_;
		std::string cmdline_;
		int connection_;
		std::string scriptpath_;
		std::string outputpath_;
		std::string errlogpath_;
		std::string output_;
		BatchSignal::type sig_;

		/* ====================  HELPERS       ======================================= */
		std::string GenerateNameByTime() const;
		std::string GenerateScriptName() const;
		const std::string LocWithHost(const std::string &) const;
		const std::string ReqResourceStr() const;
		void FillAttr(PbsAttr &);
		char GetPbsStatus() const;

		/* ====================  STEPS         ======================================= */
		ExitReason::type CreateScript();
		ExitReason::type Connect();
		ExitReason::type Submit(); /* args: attr */
		ExitReason::type Trace();
		ExitReason::type Collect();
		void Fail();
		void Exit();
		void SendFinished();
		void SendFailed();
		void SendCanceled();

		/* ====================  EVENT HANDLERS  ===================================== */
		void CancelPbsJob();
}; /* -----  end of class TorqueJob  ----- */

#endif
