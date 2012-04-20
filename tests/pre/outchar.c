/*
 * =====================================================================================
 *
 *       Filename:  out.c
 *
 *    Description:  The program producing a one-char file
 *
 *        Version:  1.0
 *        Created:  04/19/2012 09:05:49 AM
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
	FILE *fp;

	fp = fopen(argv[1], "w");
	fputc('X', fp);
	fclose(fp);

	return 0;
}				/* ----------  end of function main  ---------- */
