/*
 * =====================================================================================
 *
 *       Filename:  utility.h
 *
 *    Description:  the public functions e.g. readFile or logging are placed in class
 *    				Utility
 *
 *        Version:  0.7
 *        Created:  03/19/2012 10:43:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LIU Lu (), luliu@nudt.edu.cn
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

const int PARAM_SIZE = 1024;

class Utility {
	public:
		static int readFile(const string&, string&);
		static int deleteFile(const string&);
		static vector<string> splitStringBySpace ( const string& );
		static string intToString(int64_t);
};


#endif
