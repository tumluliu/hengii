/*
 * =====================================================================================
 *
 *       Filename:  utility.cpp
 *
 *    Description:  the implementation of class Utility
 *
 *        Version:  0.6
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
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"

void Utility::log(const string& file, const string& content)
{
	FILE *fp;
	time_t t;
	fp = fopen(file.c_str(), "a");
	if ((fp) >= 0) {
		t = time(0);
		fprintf(fp, "%s:%s\n", asctime(localtime(&t)), content.c_str());
		fclose(fp);
	}
}

int Utility::readFile(const string& file, string& content) {
	ifstream infile(file.c_str(), ios::in);
	if (!infile) {
		cout << "open file " << file << " error!" << endl;
		return -1;
	}
	content.clear();
	copy(istreambuf_iterator<char>(infile), istreambuf_iterator<char>(), back_inserter(content));
	if (isspace(content[content.size() - 1]))
		content[content.size() - 1] = ' ';
	infile.close();
	return 0;
}

int Utility::deleteFile(const string& file) {
	string cmd = "rm " + file;
	if (system(cmd.c_str()) != 0) {
		cout << "delete file " << file << " failed." << endl;
		return -1;
	}
	cout << "file " << file << " deleted." << endl;
	return 0;
}

int Utility::getCurrentDir(string& path) {
	char cpath[PARAM_SIZE];
	if (!getcwd(cpath, sizeof(cpath))) {
		return -1;
	}
	path = cpath;
	path += "/";
	return 0;
}
