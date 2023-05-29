#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "SW_matrix.h"

void SWM_initMatrix(SWM_Matrix *matrix, uint32_t rows, uint32_t columns)
{
    matrix->rows = rows;
    matrix->columns = columns;

    matrix->data = SWM_createData(rows, columns);

    // initialize to 0
    // SWM_fillMatrix(matrix, .0f);
}

void SWM_initMatrixData(SWM_Matrix *matrix, uint32_t rows, uint32_t columns, SWM_MatrixData_t data)
{
    matrix->rows = rows;
    matrix->columns = columns;

    matrix->data = data;
}

void SWM_destroyMatrix(SWM_Matrix *matrix)
{
    free(matrix->data);
}

void SWM_fillMatrix(SWM_Matrix *matrix, SWM_MatrixValue_t val)
{
    for (int i = 0, l = matrix->rows * matrix->columns; i < l; i++)
        matrix->data[i] = val;
}

// matrix operations

SWM_Matrix SWM_addMatrix(SWM_Matrix *a, SWM_Matrix *b)
{
    // safety guards
    if (a->rows != b->rows || a->columns != b->columns)
    {
        fputs("Failed to add differently sized matrices", stderr);
    }

    SWM_MatrixData_t outputData = SWM_createData(a->rows, a->columns);
    SWM_Matrix out;
    SWM_initMatrixData(&out, a->rows, a->columns, outputData);

    for (uint32_t i = 0, l = a->rows; i < l; i++)
    for (uint32_t j = 0, k = a->columns; j < k; j++)
        SWM_set(&out, i, j, SWM_at(a, i, j) + SWM_at(b, i, j));

    return out;
}

SWM_Matrix SWM_multiplyMatrix(SWM_Matrix *a, SWM_Matrix *b);
SWM_Matrix SWM_multiplyScalar(SWM_Matrix *a, float scalar);

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