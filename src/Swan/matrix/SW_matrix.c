#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SW_matrix.h"

// matrix operations

SWM_Matrix SWM_addMatrix(SWM_Matrix *a, SWM_Matrix *b)
{
    // safety guards
    if (a->rows != b->rows || a->columns != b->columns)
    {
        fputs("Failed to add differently sized matrices", stderr);

        exit(1);
    }

    SWM_MatrixData_t outputData = SWM_createData(a->rows, a->columns);
    SWM_Matrix out;
    SWM_initMatrixData(&out, a->rows, a->columns, outputData);

    for (uint32_t i = 0, l = a->rows; i < l; i++)
    for (uint32_t j = 0, k = a->columns; j < k; j++)
        SWM_set(&out, i, j, SWM_at(a, i, j) + SWM_at(b, i, j));

    return out;
}

SWM_Matrix SWM_multiplyMatrix(SWM_Matrix *a, SWM_Matrix *b)
{
    if (a->columns != b->rows)
    {
        fputs("Error multiplying two matrices of different sizes\n", stdout);
        exit(1);
    }

    // create output matrix
    SWM_Matrix out;
    SWM_initMatrix(&out, a->rows, b->columns);

    // iterate and multiply
    for (uint32_t i = 0, li = a->rows; i < li; i++)
    for (uint32_t j = 0, lj = b->columns; j < lj; j++)
    for (uint32_t k = 0, lk = b->rows; k < lk; k++)
    {
        out.data[SWM_index(&out, i, j)] += SWM_at(a, i, k) * SWM_at(b, k, j);
    }

    return out;
}

SWM_Matrix SWM_multiplyScalar(SWM_Matrix *a, float scalar)
{
    // create output matrix with data of a
    SWM_Matrix out;
    SWM_initMatrixData(&out, a->rows, a->columns, SWM_copyMatrixData(a));

    // multiply all values by scalar
    for (uint32_t i = 0, l = a->rows; i < l; i++)
    for (uint32_t j = 0, k = a->columns; j < k; j++)
        SWM_set(&out, i, j, SWM_at(&out, i, j) * scalar);

    return out;
}


// util

void SWM_printm(SWM_Matrix *matrix)
{
    if (matrix->rows == 0 || matrix->columns == 0)
    {
        fputs("Not printing empty matrix >:(", stdout);
    }
    
    fprintf(stdout, "(%d, %d)\n", matrix->rows, matrix->columns);

    for (int i = 0, li = matrix->rows; i < li; i++)
    {
        fprintf(stdout, "[%.6f", SWM_at(matrix, i, 0));
        
        for (int j = 1, lj = matrix->columns; j < lj; j++)
            fprintf(stdout, ", %.6f", SWM_at(matrix, i, j));
        
        fputs("]\n", stdout);
    }
}