#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "SW_parse.h"
#include "SW_matrix.h"
#include "SW_util.h"

#define DEBUG

// https://codereview.stackexchange.com/questions/151049/endianness-conversion-in-c
static inline uint32_t reverse32(uint32_t value)
{
    return (((value & 0x000000FF) << 24) | ((value & 0x0000FF00) <<  8) | ((value & 0x00FF0000) >>  8) | ((value & 0xFF000000) >> 24));
}

/* reads a single uint32_t from stream and flips the endianness */
static inline uint32_t freadReverse32(FILE *stream)
{
    uint32_t val;
    fread(&val, sizeof(uint32_t), 1, stream);
    val = reverse32(val);
    return val;
}


const char lightValues[10] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };

void SW_printMNISTImage(float *pointer)
{
    // add rows for newline character, add 8 just for some buffer leeway
    uint32_t buflen = MNISTIMAGESIZE*MNISTIMAGESIZE+MNISTIMAGESIZE+8;

    char output[buflen];

    uint32_t index = 0, outIndex = 0;

    for (uint32_t i = 0; i < MNISTIMAGESIZE; i++)
    {
        for (uint32_t j = 0; j < MNISTIMAGESIZE; j++)
        {
            // image data is already normalized
            uint32_t lightval = (uint32_t)round(pointer[index++] * 10.0f);

            if (lightval > 9) lightval = 9;

            output[outIndex++] = lightValues[lightval];

            if (outIndex >= buflen) { SW_FATAL("buffer overflow in image printing\n") }
        }
        output[outIndex++] = '\n';

        if (outIndex >= buflen) { SW_FATAL("buffer overflow in image printing\n") }
    }

    output[outIndex] = '\0';
    fputs(output, stdout);
}


void parseMNISTLabels(SW_MNISTData_t *data, const char *fileName)
{
    // training labels
    FILE *fLabels = fopen(fileName, "rb");

    // safety checks
    if (fLabels == NULL)
    {
        SW_FATAL("error opening training label file\n")
    }

    int32_t trainingLabelsMagicBytes;
    trainingLabelsMagicBytes = (int32_t)freadReverse32(fLabels);

    data->nLabels = (int32_t)freadReverse32(fLabels);

    data->labels = realloc(data->labels, sizeof(uint8_t) * data->nLabels);
    
    if (data->labels == NULL)
    {
        SW_FATAL("not enough memory :(\n")
    }
    
    size_t trainingLabelsNRead = fread(data->labels, sizeof(uint8_t), data->nLabels, fLabels);
    
#ifdef DEBUG
    printf("read %zu values into trainingLabels\n", trainingLabelsNRead);
#endif

    fclose(fLabels);
}

void parseMNISTImages(SW_MNISTData_t *data, const char *fileName)
{
    // training images
    FILE *fImages = fopen(fileName, "rb");

    if (fImages == NULL)
    {
        SW_FATAL("error opening training images file\n")
    }

    int32_t trainingImagesMagicBytes;
    trainingImagesMagicBytes = (int32_t)freadReverse32(fImages);

    // these two contain the image dimensions, but since those are constant we just discard those
    data->nImages = (int32_t)freadReverse32(fImages);
    freadReverse32(fImages);
    freadReverse32(fImages);

    // parse uint8_t (0...255) to float (0...1)
    
    // resize the actual images images to match tempImages
    data->images = realloc(data->images, sizeof(float) * data->nImages * MNISTIMAGESIZE * MNISTIMAGESIZE);
    
    if (data->images == NULL)
    {
        SW_FATAL("not enough memory :(\n")
    }

    // convert uint8_t (0...255) data to float (0...1)

    // copy data into array of different type (uint8_t -> float)
    fread((uint8_t*)data->images, sizeof(uint8_t), data->nImages * MNISTIMAGESIZE * MNISTIMAGESIZE, fImages);

    // convert the raw uint8_t data to float data

    // set the data pointer to the last pixel in the uint8_t data stored in the float array
    uint8_t *dataPointer = (uint8_t*)(data->images) + (sizeof(uint8_t) * (data->nImages*MNISTIMAGESIZE*MNISTIMAGESIZE - 1));

    uint32_t nPixelsRead = 0;

    // loop through the data backwards and put the corresponding normalized value at the correct place in the float array
    for (uint32_t i = data->nImages*MNISTIMAGESIZE*MNISTIMAGESIZE-1; i > 0; i--, nPixelsRead++)
    {
        data->images[i] = (float)(*dataPointer) / 255.0f;
        dataPointer -= sizeof(uint8_t);
    }

#ifdef DEBUG
    printf("read %u pixels into trainingImages\n", nPixelsRead);
#endif

    fclose(fImages);
}

void SW_unloadMNIST(SW_MNISTData_t *data)
{
    free(data->images);
    free(data->labels);
}


void SW_parseMNIST(SW_MNISTData_t *trainingData, SW_MNISTData_t *testData)
{
    // this function totally discards magic bytes because idk
    parseMNISTImages(trainingData, "MNISTdataset/train-images-idx3-ubyte");
    parseMNISTLabels(trainingData, "MNISTdataset/train-labels-idx1-ubyte");
    parseMNISTImages(testData, "MNISTdataset/t10k-images-idx3-ubyte");
    parseMNISTLabels(testData, "MNISTdataset/t10k-labels-idx1-ubyte");
}

SWM_Matrix SW_MNISTImageToMatrix(float *imagePointer)
{
    SWM_Matrix out;
    SWM_init(&out, 1, MNISTIMAGESIZE*MNISTIMAGESIZE);
    memcpy(out.data, imagePointer, sizeof(float) * MNISTIMAGESIZE * MNISTIMAGESIZE);
    return out;
}