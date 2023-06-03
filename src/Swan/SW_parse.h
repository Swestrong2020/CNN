#ifndef SW_PARSE_H
#define SW_PARSE_H

#include <stdint.h>

#include "SW_matrix.h"

void SW_parseMNIST(
    uint32_t *_nTrainingLabels, uint32_t *_nTrainingImages, uint32_t *_nTestLabels, uint32_t *_nTestImages,
    uint8_t **_trainingLabels, uint8_t **_trainingImages, uint8_t **_testLabels, uint8_t **_testImages
);

/* prints digit in mnist dataset from first row * column bytes in pointer */
void SW_printMNISTImage(uint8_t *pointer, int32_t rows, int32_t columns);

/* copies the first image at imagePointer to an SWM_Matrix. ret freed by caller */
SWM_Matrix SW_MNISTImageToMatrix(uint8_t *imagePointer, int32_t rows, int32_t columns);

#endif // SW_PARSE_H