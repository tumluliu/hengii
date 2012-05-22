/*
 * =====================================================================================
 *
 *       Filename:  utility.h
 *
 *    Description:  the public functions e.g. readFile or logging are placed in class
 *    				Utility
 *
 *        Version:  1.0
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

namespace util {
	int readFile(const std::string&, std::string&);
	int deleteFile(const std::string&);
	std::string intToString(int64_t);
	std::string getCurrentTime();
	int stoi(const std::string &s);
}


#endif
