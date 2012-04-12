/*
 * =====================================================================================
 *
 *       Filename:  hg_pbsattr.h
 *
 *    Description:  This file contains functions to make pbs attrl management easier
 *
 *        Version:  1.0
 *        Created:  04/11/2012 08:25:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pbs_error.h>
#include <pbs_ifl.h>

#ifndef MONITOR_HG_PBSATTR_H_
#define MONITOR_HG_PBSATTR_H_

void convertToStdTime( char *result, time_t t ); 

void makeAttrNames( struct attrl* );
void freeAttrNames( struct attrl* );
void getAttrValue( char *result, struct attrl *attr, char *name ); 

#endif
