/*
 * =====================================================================================
 *
 *       Filename:  utility.cpp
 *
 *    Description:  the implementation of class Utility
 *
 *        Version:  1.0
 *        Created:  03/19/2012 10:52:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "utility.h"
#include "log.h"

namespace util {
	using std::string;
	using std::vector;

	int readFile(const string& file, string& content) {
		std::ifstream infile(file.c_str(), std::ios::in);
		if (!infile) {
			Log().Error() << "unable to open file " << file;
			return -1;
		}
		content.clear();
		copy(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>(), back_inserter(content));
		if (isspace(content[content.size() - 1]))
			content[content.size() - 1] = ' ';
		infile.close();
		return 0;
	}

	int deleteFile(const string& file) {
		string cmd = "rm " + file;
		if (system(cmd.c_str()) != 0) {
			Log().Error() << "unable to delete file " << file;
			return -1;
		}
		Log().Info() << "file " << file << " deleted.";
		return 0;
	}

	string intToString(int64_t i) {
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	string getCurrentTime() {
		const int TIME_LENGTH = 80;
		char result[TIME_LENGTH];
		time_t t = time(0);

		strftime( result, TIME_LENGTH, "%Y-%m-%d %X", localtime( &t ) );

		return string(result);
	}

	int stoi(const string &s) { return atoi(s.c_str()); }
}
