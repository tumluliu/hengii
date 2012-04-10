/*
 * =====================================================================================
 *
 *       Filename:  hello_to.c
 *
 *    Description:  A hello world contains params(normal, with switch, optional)
 *
 *        Version:  1.0
 *        Created:  04/09/2012 09:21:56 AM
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
#include "mpi.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  The entrance of the world  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ) {
	int myid, numprocs;
	char *target = argv[1];
	char *prefix = "";
	char *egg = "";

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);

	if (argc > 2) {
		prefix = argv[2];
		if (argc > 4 && strcmp(argv[3], "-e") == 0) {
			egg = argv[4];
		}
	}

	/* Here the outputs of all processes are the same, for the convienence of result checking */
	printf ("Hello %s %s!!!%s\n", prefix, target, egg);
	MPI_Finalize ();

	return 0;
}				/* ----------  end of function main  ---------- */
