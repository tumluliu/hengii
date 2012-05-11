/*
 * =====================================================================================
 *
 *       Filename:  appoption.h
 *
 *    Description:  
 *
 *        Version:  1.0
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

class AppOption
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		AppOption(const std::vector<std::string> &);

		/* ====================  ACCESSORS     ======================================= */
		std::string get_name() const;
		std::string get_cmdswitch() const; 
		std::string get_defaultvalue() const;

		bool IsOptional() const;

		/* ====================  TOOLS         ======================================= */
		static bool IsUnknown(const std::vector<AppOption> &, 
				const std::string &);

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string name_;
		bool isoptional_;
		std::string cmdswitch_; 
		std::string defaultvalue_;

};

#endif
