#ifndef SW_PARSE_H
#define SW_PARSE_H

#include <stdint.h>

#include "SW_matrix.h"

#define MNISTIMAGESIZE 28

typedef struct SW_MNISTData_t
{
    uint32_t nLabels, nImages;
    float *labels, *images;
} SW_MNISTData_t;

/* parses the *entire* mnist data set *into* function arguments. Normalizes pixel data (0...255 -> 0...1)*/
void SW_parseMNIST(SW_MNISTData_t *trainingData, SW_MNISTData_t *testData);

/* unloads mnist data */
void SW_unloadMNIST(SW_MNISTData_t *data);

/* prints *parsed* digit in mnist dataset from first row * column bytes in pointer */
void SW_printMNISTImage(float *pointer);

/* copies the first image at imagePointer to an SWM_Matrix. ret freed by caller */
SWM_Matrix SW_MNISTImageToMatrix(float *imagePointer);

#endif // SW_PARSE_H