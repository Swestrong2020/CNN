#ifndef SW_MATRIX_H
#define SW_MATRIX_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* SW_MatrixData_t data[row][column] */
typedef float SWM_MatrixValue_t;
typedef SWM_MatrixValue_t* SWM_MatrixData_t;

typedef struct SWM_Matrix
{

    SWM_MatrixData_t data;
    uint32_t rows, columns;

} SWM_Matrix;

static inline uint32_t SWM_index(SWM_Matrix *matrix, uint32_t row, uint32_t col)
{
    return (row * matrix->columns + col);
}

static inline SWM_MatrixValue_t SWM_at(SWM_Matrix *matrix, uint32_t row, uint32_t col)
{
    return matrix->data[SWM_index(matrix, row, col)];
}

static inline void SWM_set(SWM_Matrix *matrix, uint32_t row, uint32_t column, SWM_MatrixValue_t value)
{
    matrix->data[SWM_index(matrix, row, column)] = value;
}

static inline SWM_MatrixData_t SWM_createData(uint32_t rows, uint32_t columns)
{
    SWM_MatrixData_t data = (SWM_MatrixData_t) malloc(sizeof(SWM_MatrixValue_t) * rows * columns);
    if (data == NULL) { fputs("Error allocating matrix data\n", stdout); exit(1); }
    else { return data; }
}

static inline void SWM_initMatrix(SWM_Matrix *matrix, uint32_t rows, uint32_t columns)
{
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->data = SWM_createData(rows, columns);
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

SWM_MatrixData_t SWM_copyMatrixData(SWM_Matrix *matrix) /* ret freed by caller */
{
    SWM_MatrixData_t data = SWM_createData(matrix->rows, matrix->columns);
    memcpy(data, matrix->data, matrix->columns * matrix->rows * sizeof(SWM_MatrixValue_t));
    return data;
}

// matrix operations

SWM_Matrix SWM_addMatrix(SWM_Matrix *a, SWM_Matrix *b); /* ret freed by caller */
SWM_Matrix SWM_multiplyMatrix(SWM_Matrix *a, SWM_Matrix *b); /* ret freed by caller */
SWM_Matrix SWM_multiplyScalar(SWM_Matrix *a, SWM_MatrixValue_t scalar); /* ret freed by caller */

// util

void SWM_printm(SWM_Matrix *matrix);

#endif // SW_MATRIX_H