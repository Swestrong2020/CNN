#ifndef SW_PARSE_H
#define SW_PARSE_H

#include <stdint.h>

#include "SW_matrix.h"

#define MNISTIMAGESIZE 28

/* parses the *entire* mnist data set into arguments. Normalizes pixel data (0...255 -> 0...1)*/
void SW_parseMNIST(
    uint32_t *_nTrainingLabels, uint32_t *_nTrainingImages, uint32_t *_nTestLabels, uint32_t *_nTestImages,
    uint8_t **_trainingLabels, float **_trainingImages, uint8_t **_testLabels, float **_testImages
);

/* prints *parsed* digit in mnist dataset from first row * column bytes in pointer */
void SW_printMNISTImage(float *pointer);

/* copies the first image at imagePointer to an SWM_Matrix. ret freed by caller */
SWM_Matrix SW_MNISTImageToMatrix(float *imagePointer);

#endif // SW_PARSE_H