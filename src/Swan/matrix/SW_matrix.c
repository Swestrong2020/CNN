#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SW_matrix.h"

// matrix operations

void SWM_addMatrixD(SWM_Matrix *a, SWM_Matrix *b, SWM_Matrix *dest)
{
    if (dest->rows != a->rows || dest->columns != a->columns)
    {
       SWM_FATAL("Destination matrix not of the right size\n")
    }

    // safety guards
    if (a->rows != b->rows || a->columns != b->columns)
    {
        SWM_FATAL("Failed to add differently sized matrices\n")
    }

    for (uint32_t i = 0, l = dest->rows; i < l; i++)
    for (uint32_t j = 0, k = dest->columns; j < k; j++)
        SWM_set(dest, i, j, SWM_get(a, i, j) + SWM_get(b, i, j));
}

void SWM_multiplyMatrixD(SWM_Matrix *a, SWM_Matrix *b, SWM_Matrix *dest)
{
    if (dest->rows != a->rows || dest->columns != b->columns)
    {
        SWM_FATAL("Destination matrix not of the right size\n")
    }

    if (a->columns != b->rows)
    {
        SWM_FATAL("Error multiplying two matrices of different sizes\n")
    }

    // iterate and multiply
    // SLOW METHOD DAYUM
    for (uint32_t i = 0, li = a->rows; i < li; i++)
    for (uint32_t j = 0, lj = b->columns; j < lj; j++)
    for (uint32_t k = 0, lk = b->rows; k < lk; k++)
    {
        dest->data[SWM_index(dest, i, j)] = SWM_get(a, i, k) * SWM_get(b, k, j);
    }
}

void SWM_multiplyScalarD(SWM_Matrix *a, float scalar, SWM_Matrix *dest)
{
    // multiply all values by scalar
    for (uint32_t i = 0, l = a->rows; i < l; i++)
    for (uint32_t j = 0, k = a->columns; j < k; j++)
        SWM_set(dest, i, j, SWM_get(a, i, j) * scalar);
}

void SWM_transposeD(SWM_Matrix *a, SWM_Matrix *dest)
{
    if (dest->columns != a->rows || dest->rows != a->columns)
    {
        SWM_FATAL("Error transposing, destination matrix wrong size\n")
    }

    for (uint32_t i = 0, li = a->columns; i < li; i++)
    for (uint32_t j = 0, lj = a->rows; j < lj; j++)
    {
        dest->data[SWM_index(dest, i, j)] = SWM_get(a, j, i);
    }
}


// util

void SWM_printm(SWM_Matrix *matrix)
{
    if (matrix->rows == 0 || matrix->columns == 0)
    {
        SWM_FATAL("Not printing empty matrix >:(\n")
    }
    
    fprintf(stdout, "(%d, %d)\n", matrix->rows, matrix->columns);

    for (int i = 0, li = matrix->rows; i < li; i++)
    {
        fprintf(stdout, "[%.6f", SWM_get(matrix, i, 0));
        
        for (int j = 1, lj = matrix->columns; j < lj; j++)
            fprintf(stdout, ", %.6f", SWM_get(matrix, i, j));
        
        fputs("]\n", stdout);
    }
}