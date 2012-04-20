/*
 * =====================================================================================
 *
 *       Filename:  torquejob.h
 *
 *    Description:  the declaration of job class submitted to Torque PBS.
 *
 *        Version:  0.9
 *        Created:  03/17/2012 03:38:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TORQUEJOB_H_
#define _TORQUEJOB_H_

#include <string>

using namespace std;

const string PBS_ERRLOG_FILE_EXT = ".pbserr";
const string PBS_OUTPUT_FILE_EXT = ".pbsout";

class TorqueJob{
	private:
		string id;
		int processCount;
		int connection;
		string status;
		string scriptPath;
		string outputPath;
		string errlogPath;
		string cmdline;
		string output;
		string generateNameByTime();
	public:
		TorqueJob(): id(""), processCount(0), connection(-1), scriptPath(""), outputPath(""), cmdline("") { }
		int submit();
		int collect();
		int setProcessCount(int);
		void setCmdline(const string&);
		string getCmdline() const;
		string getStatus() const;
		int getConnection() const;
		string getId() const;
		string getOutput() const;
};

#endif
