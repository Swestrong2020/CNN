#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "SW_parse.h"
#include "SW_matrix.h"

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

            if (outIndex >= buflen) { fputs("buffer overflow in image printing\n", stderr); abort(); }
        }
        output[outIndex++] = '\n';

        if (outIndex >= buflen) { fputs("buffer overflow in image printing\n", stderr); abort(); }
    }

    output[outIndex] = '\0';
    fputs(output, stdout);
}


void parseMNISTLabels(SW_MNISTData_t *data, const char *fileName)
{
    // training labels
    FILE *fTrainingLabels = fopen(fileName, "rb");

    // safety checks
    if (fTrainingLabels == NULL)
    {
        fputs("error opening training label file\n", stderr);
        abort();
    }

    int32_t trainingLabelsMagicBytes;
    trainingLabelsMagicBytes = (int32_t)freadReverse32(fTrainingLabels);

    data->nLabels = (int32_t)freadReverse32(fTrainingLabels);

    data->labels = realloc(data->labels, sizeof(uint8_t) * data->nLabels);
    
    if (data->labels == NULL)
    {
        fputs("not enough memory :(\n", stderr);
        abort();
    }
    
    size_t trainingLabelsNRead = fread(data->labels, sizeof(uint8_t), data->nLabels, fTrainingLabels);
    
#ifdef DEBUG
    printf("read %zu values into trainingLabels\n", trainingLabelsNRead);
#endif

    fclose(fTrainingLabels);
}

void parseMNISTImages(SW_MNISTData_t *data, const char *fileName)
{
    // training images
    FILE *fTrainingImages = fopen(fileName, "rb");

    if (fTrainingImages == NULL)
    {
        fputs("error opening training images file\n", stderr);
        abort();
    }

    int32_t trainingImagesMagicBytes;
    trainingImagesMagicBytes = (int32_t)freadReverse32(fTrainingImages);

    // these two contain the image dimensions, but since those are constant we just discard those
    data->nImages = (int32_t)freadReverse32(fTrainingImages);
    freadReverse32(fTrainingImages);
    freadReverse32(fTrainingImages);

    // parse uint8_t (0...255) to float (0...1)
    
    // resize the actual images images to match tempImages
    data->images = realloc(data->images, sizeof(float) * data->nImages * MNISTIMAGESIZE * MNISTIMAGESIZE);
    
    if (data->images == NULL)
    {
        fputs("not enough memory :(\n", stderr);
        abort();
    }

    // convert data
    // pretty slow because of the frequent read/writes but that's not really a problem since
    // it's a preprocessing step
    uint32_t trainingImagesRead = 0;
    for (uint32_t l = data->nImages*MNISTIMAGESIZE*MNISTIMAGESIZE; trainingImagesRead < l; trainingImagesRead++)
    {
        uint8_t pixelData;
        fread(&pixelData, sizeof(uint8_t), 1, fTrainingImages);
        data->images[trainingImagesRead] = (float)pixelData / 255.0f;
    }

#ifdef DEBUG
    printf("read %zu pixels into trainingImages\n", trainingImagesRead);
#endif

    fclose(fTrainingImages);
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
    SWM_initMatrix(&out, 1, MNISTIMAGESIZE*MNISTIMAGESIZE);
    memcpy(out.data, imagePointer, sizeof(float) * MNISTIMAGESIZE * MNISTIMAGESIZE);
    return out;
}