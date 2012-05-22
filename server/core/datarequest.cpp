/*
 * =====================================================================================
 *
 *       Filename:  datarequest.cpp
 *
 *    Description:  specify a request to a data
 *
 *        Version:  1.0
 *        Created:  05/18/2012 04:13:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "datarequest.h"

DataRequest::DataRequest(const std::string &datauri, Access::type how,
		DataType::type t) : datauri_(datauri), how_(how), datatype_(t) {
}

std::string DataRequest::get_datauri() const {
	return datauri_;
}

Access::type DataRequest::get_accesstype() const {
	return how_;
}

DataType::type DataRequest::get_datatype() const {
	return datatype_;
}
