/*
 * =====================================================================================
 *
 *       Filename:  pbsattr.h
 *
 *    Description:  pbs attr builder
 *
 *        Version:  1.0
 *        Created:  05/06/2012 08:01:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PBSATTR_H_
#define  PBSATTR_H_

#include <map>
#include <string>

extern "C" {
#include <pbs_ifl.h>
}

/*
 * =====================================================================================
 *        Class:  PbsAttr
 *  Description:  He helps to build and destroy the pbs attr struct. memory safe.
 * =====================================================================================
 */
class PbsAttr
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* args: params(except for resources) k-v pair; resources k-v pair */
		PbsAttr();            /* constructor */
		~PbsAttr();

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */
		/* args: name; value */
		void Add(const char *, const std::string&);
		void AddResource(const std::string&, const std::string&);

		/* ====================  c style attr list lifecycle  ======================== */
		/* the result pointer will be destroyed when destructor is invoked */
		struct attropl *MakeCAttrl(); 
		void DestroyCAttrl(struct attropl *);

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::map<std::string, std::string> params_;
		std::map<std::string, std::string> resources_;
		struct attropl *cache_;

		/* ====================  HELPERS       ======================================= */
		/* args: pointer to last element, name; 
		 * resource name(null if not resource param); value.
		 * return: pointer to the new element*/
		struct attropl *AddCAttrEle(struct attropl *, 
				const std::string &, const std::string &,
				const std::string &);

		/* ====================  Should be impl but not now  ========================= */
		PbsAttr(const PbsAttr &);
		PbsAttr & operator=(const PbsAttr &);
}; /* -----  end of class PbsAttr  ----- */

#endif
