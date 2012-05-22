/*
 * =====================================================================================
 *
 *       Filename:  datarequest.h
 *
 *    Description:  specify a request to a data
 *
 *        Version:  1.0
 *        Created:  05/18/2012 04:08:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  DATAREQUEST_H_
#define  DATAREQUEST_H_

#include <string>

struct Access {
	enum type {
		READ = 1,
		UPDATE = 2,
		CREATE = 3
	};
};

/* WARNING: It is here because the raster and feature data are dealt seperately in 
 * database. It is ugly but now beyond my control. by YANG Anran @ 2012.5.19 */
struct DataType {
	enum type {
		FEATURE = 1,
		RASTER = 2
	};
};

/*
 * =====================================================================================
 *        Class:  DataRequest(value type)
 *  Description:  He describe a data and how it is to be used
 * =====================================================================================
 */
class DataRequest
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		/* WARNING: Data type is here because the raster and feature data are dealt
		 * seperately in database. It is ugly but now beyond my control. 
		 * by YANG Anran @ 2012.5.19 */ 
		/* WARNING: Here uri is not REAL uri, because the engine doesn't know it. it 
		 * is what transfered in job's option list to uniquely(considering the data 
		 * type is given) identify a dataset. by YANG Anran @ 2012.5.19 */
		/* constructor, args:data uri;access type;data type */
		DataRequest(const std::string &, Access::type, DataType::type);

		/* ====================  ACCESSORS     ======================================= */
		std::string get_datauri() const;
		Access::type get_accesstype() const;
		DataType::type get_datatype() const;

		/* ====================  MUTATORS      ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		std::string datauri_;
		Access::type how_;
		DataType::type datatype_;
}; /* -----  end of class DataRequest  ----- */

#endif
