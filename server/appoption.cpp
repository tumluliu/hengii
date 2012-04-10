/*
 * =====================================================================================
 *
 *       Filename:  appoption.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/2012 11:14:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "appoption.h"

AppOption::AppOption() {
	name = "";
	isOptional = false;
	cmdSwitch = "";
	defaultValue = "";
}                             /* constructor */

AppOption::AppOption(vector<string> s) {
	name = s[0];
	isOptional = s.size() > 1 && s[1] == "o" ? true : false;
	cmdSwitch = s.size() > 2 ? s[2] : "" ;
	defaultValue = s.size() > 3 ? s[3] : "";
}

bool AppOption::isUnknown(vector<AppOption> metas, string offered) {
	unsigned int i;
	for (i = 0; i < metas.size(); i++) {
		if(metas[i].name == offered) {
			return false;
		}
	}
	return true;
}
