/*
 * =====================================================================================
 *
 *       Filename:  in.c
 *
 *    Description:  The program reading a one-char file and print it out
 *
 *        Version:  1.0
 *        Created:  04/19/2012 09:06:18 AM
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
	char c;

	fp = fopen(argv[1], "r");
	c = fgetc(fp);
	sleep(3);
	putchar(c);
	fclose(fp);

	return 0;
}				/* ----------  end of function main  ---------- */
