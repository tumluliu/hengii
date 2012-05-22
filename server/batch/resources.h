/*
 * =====================================================================================
 *
 *       Filename:  resources.h
 *
 *    Description:  pbs resources
 *
 *        Version:  1.0
 *        Created:  05/07/2012 10:16:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  RESOURCES_H_
#define  RESOURCES_H_

/*
 * =====================================================================================
 *        Class:  Resources
 *  Description:  He represent some resources
 * =====================================================================================
 */
class Resources
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Resources();                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		int get_nodes() const;
		int get_ppn() const;

		/* ====================  AUTO DISTRIBUTE    ================================== */
		/* args: process count */
		void Request(int);
		/* ====================  MUTATORS      ======================================= */
		 
	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		int nodes_;
		int ppn_;
}; /* -----  end of class Resources  ----- */

#endif
