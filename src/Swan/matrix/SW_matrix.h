#ifndef SW_MATRIX_H
#define SW_MATRIX_H

#include <stdint.h>

/* SW_MatrixData_t data[row][column] */
typedef float** SW_MatrixData_t;

typedef struct SW_Matrix
{

    SW_MatrixData_t data;
    uint32_t rows, columns;

} SW_Matrix;

void SW_initMatrixEmpty(SW_Matrix *matrix, uint32_t rows, uint32_t columns);
void SW_initMatrixData(SW_Matrix *matrix, uint32_t rows, uint32_t columns, float **data);
void SW_destroyMatrix(SW_Matrix *matrix);
void SW_fillMatrix(SW_Matrix *matrix, float val);

// matrix operations

SW_Matrix SW_addMatrix(SW_Matrix *a, SW_Matrix *b);
SW_Matrix SW_multiplyMatrix(SW_Matrix *a, SW_Matrix *b);
SW_Matrix SW_multiplyScalar(SW_Matrix *a, float scalar);

#endif // SW_MATRIX_H