/*
 * =====================================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  The Hello world program of MPI
 *
 *        Version:  1.0
 *        Created:  03/18/2012 08:14:52 PM
 *       Revision:  none
 *       Compiler:  gcc 4.5.2 / openmpi 1.4.3
 *
 *         Author:  YANG Anran (), 08to09@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  The entrance of the world  
 * =====================================================================================
 */
int main (int argc, char **argv)
{
	int myid, numprocs;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	printf ( "Hello" );
	sleep(5);
	printf ( "oooo!!!!\n" );
	MPI_Finalize ();
	return 0;
}
