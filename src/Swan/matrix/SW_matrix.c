#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "SW_matrix.h"

void SW_initMatrixEmpty(SW_Matrix *matrix, uint32_t rows, uint32_t columns)
{
    matrix->rows = rows;
    matrix->columns = columns;

    matrix->data = (SW_MatrixData_t) malloc(sizeof(float) * rows);
    for (uint32_t i = 0; i < rows; i++)
        matrix->data[i] = malloc(sizeof(float) * columns);

    // initialize to 0
    // SW_fillMatrix(matrix, .0f);
}

void SW_initMatrixData(SW_Matrix *matrix, uint32_t rows, uint32_t columns, float **data)
{
    matrix->rows = rows;
    matrix->columns = columns;

    matrix->data = data;
}

void SW_destroyMatrix(SW_Matrix *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
        free(matrix->data[i]);
    free(matrix->data);
}

void SW_fillMatrix(SW_Matrix *matrix, float val)
{
    for (int i = 0; i < matrix->rows; i++)
    for (int j = 0; j < matrix->columns; j++)
        matrix->data[i][j] = val;
}

// matrix operations

SW_Matrix SW_addMatrix(SW_Matrix *a, SW_Matrix *b)
{
    // safety guards
    if (a->rows != b->rows || a->columns != b->columns)
    {
        fputs("Failed to add differently sized matrices", stderr);
    }

    
}

SW_Matrix SW_multiplyMatrix(SW_Matrix *a, SW_Matrix *b);
SW_Matrix SW_multiplyScalar(SW_Matrix *a, float scalar);