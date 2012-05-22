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
#include <map>

class AppOption
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* args: name;isopt;cmdswitch;defaultvalue */
		AppOption(const std::string &, bool, const std::string &,
				const std::string &);
		/* Here given a empty constructor to offer a convienent way to create a 
		 * to-be-thrown temp var, mainly used when as out parameters */
		AppOption();

		/* ====================  ACCESSORS     ======================================= */
		std::string get_name() const;
		std::string get_cmdswitch() const; 
		std::string get_defaultvalue() const;
		bool IsOptional() const;
		/* args:(out)value;key of extra info queried
		 * return: signal, 0 if found, -1 otherwise */
		int get_extra(std::string &, const std::string &) const;

		/* ====================  MUTATORS      ======================================= */
		void set_name(const std::string &);
		void set_cmdswitch(const std::string &); 
		void set_defaultvalue(const std::string &);
		void set_optional(bool);

		/* args: key;value */
		void add_extra(const std::string &, const std::string &);

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string name_;
		bool isoptional_;
		std::string cmdswitch_; 
		std::string defaultvalue_;
		std::map<std::string, std::string> extra_;
};

#endif
