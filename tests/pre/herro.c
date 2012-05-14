/*
 * =====================================================================================
 *
 *       Filename:  herro.c
 *
 *    Description:  a program produces stderr output
 *
 *        Version:  1.0
 *        Created:  05/14/2012 11:04:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  The entrance of the world  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ) {
    fprintf(stderr, "I'm from stderr, herro!\n");
	return 0;
}				/* ----------  end of function main  ---------- */
