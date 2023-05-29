#ifndef SW_MATRIX_H
#define SW_MATRIX_H

#include <stdint.h>
#include <stdlib.h>

/* SW_MatrixData_t data[row][column] */
typedef float* SWM_MatrixData_t;
typedef float SWM_MatrixValue_t;

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

static inline void SWM_set(SWM_Matrix *matrix, uint32_t row, uint32_t column, float value)
{
    matrix->data[SWM_index(matrix, row, column)] = value;
}

static inline SWM_MatrixData_t SWM_createData(uint32_t rows, uint32_t columns)
{
    return (SWM_MatrixData_t) malloc(sizeof(float) * rows * columns);
}

void SWM_initMatrix(SWM_Matrix *matrix, uint32_t rows, uint32_t columns);
void SWM_initMatrixData(SWM_Matrix *matrix, uint32_t rows, uint32_t columns, SWM_MatrixData_t data);
void SWM_destroyMatrix(SWM_Matrix *matrix);
void SWM_fillMatrix(SWM_Matrix *matrix, float val);

// matrix operations

SWM_Matrix SWM_addMatrix(SWM_Matrix *a, SWM_Matrix *b); /* ret handled by caller */
SWM_Matrix SWM_multiplyMatrix(SWM_Matrix *a, SWM_Matrix *b); /* ret handled by caller */
SWM_Matrix SWM_multiplyScalar(SWM_Matrix *a, float scalar); /* ret handled by caller */

// util

void SWM_printm(SWM_Matrix *matrix);

#endif // SW_MATRIX_H