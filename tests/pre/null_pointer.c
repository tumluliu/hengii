/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdio.h>
#include "mpi.h"

typedef struct PointCoordinate {
    double x;
    double y;
} Point;

int main( int argc, char *argv[] )
{
    int rank;
    int size;
    
    MPI_Init( 0, 0 );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Point *p = NULL;
    printf("Try getting member values from null pointer...");
    p->x = 3.2;
    p->y = 4.8;
    MPI_Finalize();
    return 0;
}


