/*
 * =====================================================================================
 *
 *       Filename:  appoption.h
 *
 *    Description:  
 *
 *        Version:  0.8
 *        Created:  04/10/2012 11:21:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  APPOPTION_H_
#define  APPOPTION_H_

#include <string>
#include <vector>

using namespace std;

class AppOption
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		AppOption();                             /* constructor */
		AppOption( vector<string> );

		/* ====================  ACCESSORS     ======================================= */
		string name;
		bool isOptional;
		string cmdSwitch; 
		string defaultValue;

		/* ====================  TOOLS         ======================================= */
		static bool isUnknown( vector<AppOption> , string);

	private:
		/* ====================  DATA MEMBERS  ======================================= */

};

#endif
