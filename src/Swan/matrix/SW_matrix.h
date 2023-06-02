#ifndef SW_MATRIX_H
#define SW_MATRIX_H

#include <stdio.h>
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
    if (data == NULL) { fputs("Error allocating matrix data\n", stderr); abort(); }
    else { return data; }
}

static inline void SWM_initMatrix(SWM_Matrix *matrix, uint32_t rows, uint32_t columns)
{
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->data = SWM_createData(rows, columns);
}

static inline void SWM_initMatrixData(SWM_Matrix *matrix, uint32_t rows, uint32_t columns, SWM_MatrixData_t data)
{
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->data = data;
}

static inline void SWM_destroyMatrix(SWM_Matrix *matrix)
{
    free(matrix->data);
}

static inline void SWM_fillMatrix(SWM_Matrix *matrix, float scalar)
{
    for (int i = 0, l = matrix->rows * matrix->columns; i < l; i++)
        matrix->data[i] = scalar;
}

// matrix operations

void SWM_addMatrixD(SWM_Matrix *a, SWM_Matrix *b, SWM_Matrix *dest);
void SWM_multiplyMatrixD(SWM_Matrix *a, SWM_Matrix *b, SWM_Matrix *dest);
void SWM_multiplyScalarD(SWM_Matrix *a, float scalar, SWM_Matrix *dest);

static inline SWM_Matrix SWM_addMatrix(SWM_Matrix *a, SWM_Matrix *b)
{
    SWM_Matrix out;
    SWM_initMatrix(&out, a->rows, a->columns);
    SWM_addMatrixD(a, b, &out);
    return out;
}

static inline SWM_Matrix SWM_multiplyMatrix(SWM_Matrix *a, SWM_Matrix *b)
{
    SWM_Matrix out;
    SWM_initMatrix(&out, a->rows, b->columns);
    SWM_multiplyMatrixD(a, b, &out);
    return out;
}

static inline SWM_Matrix SWM_multiplyScalar(SWM_Matrix *a, float scalar)
{
    SWM_Matrix out;
    SWM_initMatrix(&out, a->rows, a->columns);
    SWM_multiplyScalarD(a, scalar, &out);
    return out;
}

static inline void SWM_applyFunction(SWM_Matrix *matrix, float (*func)(float)) /* for every value i in the matrix, set i to func(i) */
{
    for (uint32_t i = 0, l = matrix->rows * matrix->columns; i < l; i++)
        matrix->data[i] = func(matrix->data[i]);
}
// util

void SWM_printm(SWM_Matrix *matrix);

#endif // SW_MATRIX_H