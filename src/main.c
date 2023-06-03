// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <string.h>

// #include "Swan/Swan.h"

// int main(void)
// {
//     uint32_t TestImageID = 0;

//     // Load the MNIST dataset
//     // Some of this is a bit of a mess, because I didn't want to take the effort to fix big and little endianness issues
//     FILE *MNISTLabelFile = fopen("MNISTdataset/train-labels-idx1-ubyte", "rb");

//     int32_t MNISTLabelsMagicBytes;
//     fread(&MNISTLabelsMagicBytes, sizeof(int32_t), 1, MNISTLabelFile);

//     // Sure read unnecesary data into some temp value nothing can go wrong with that (fseek was too hard)
//     float TEMP;

//     // Magic mumbers are different from the ones provided by the dataset, because of endianness stupidness
//     if (MNISTLabelsMagicBytes != 17301504 && MNISTLabelsMagicBytes != 2049)
//     {
//         puts("Looks like something's wrong with the provided MNIST dataset (label)");
//         printf("%i", MNISTLabelsMagicBytes);
//         return -1;
//     }

//     fread(&TEMP, sizeof(uint32_t), 1, MNISTLabelFile);

//     uint8_t *MNISTLabels = malloc(sizeof(uint8_t) * 6000);

//     fread(MNISTLabels, sizeof(uint8_t), 6000, MNISTLabelFile);

//     fclose(MNISTLabelFile);

//     FILE *MNISTImageFile = fopen("MNISTdataset/train-images-idx3-ubyte", "rb");

//     int32_t MNISTImagesMagicBytes;
//     fread(&MNISTImagesMagicBytes, sizeof(int32_t), 1, MNISTImageFile);

//     if (MNISTImagesMagicBytes != 50855936 && MNISTImagesMagicBytes != 2051)
//     {
//         puts("Looks like something's wrong with the provided MNIST dataset (image)");
//         return -1;
//     }

//     fread(&TEMP, sizeof(uint32_t), 1, MNISTImageFile);

//     float **ImageData = malloc(sizeof(float *) * 6000);

//     for (int32_t i = 0; i < 6000; i++)
//     {
//         fread(&TEMP, sizeof(int32_t), 1, MNISTImageFile);
//         fread(&TEMP, sizeof(int32_t), 1, MNISTImageFile);

//         ImageData[i] = malloc(sizeof(float) * 28 * 28);

//         for (uint32_t j = 0; j < 28 * 28; j++)
//         {
//             uint8_t Val;
//             fread(&Val, sizeof(uint8_t), 1, MNISTImageFile);
//             ImageData[i][j] = Val / 256.0f;
//         }
//     }

//     fclose(MNISTImageFile);

//     // Debug, test if things loaded succesfully
//     for (int32_t row = 0; row < 28; row++)
//     {
//         for (int32_t column = 0; column < 28; column++)
//         {
//             if (ImageData[TestImageID][row * 28 + column] > 0.66f)
//                 putchar('x');
//             else if (ImageData[TestImageID][row * 28 + column] > 0.33f)
//                 putchar('-');
//             else
//                 putchar(' ');
//         }

//         putchar('\n');
//     }

//     // Example network
//     SW_Network network;

//     SW_InitNetwork(&network);

//     SW_AddNetworkLayer(&network, 28 * 28, SW_ACTIVATION_FUNCTION_RELU);
//     SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
//     SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
//     SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_SIGMOID);

//     SW_RandomizeNetwork(&network);
    
//     // SW_LoadNetwork(&network, "savednetwork");

//     // Calculate all the correct outputs to train the network with
//     float **CorrectOutput = malloc(sizeof(float *) * 6000);

//     for (uint32_t i = 0; i < 6000; i++)
//     {
//         CorrectOutput[i] = calloc(10, sizeof(float));
//         CorrectOutput[i][MNISTLabels[i]] = 1.0f;
//     }

//     // A testing loop that shows the loss every so often
//     printf("Loss: %.20f\n", SW_CalculateLoss(&network, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, ImageData[TestImageID], CorrectOutput[TestImageID]));

//     for (uint32_t i = 0; i < 5000; i++)
//     {
//         SW_TrainNeuralNetwork(&network, ImageData, CorrectOutput, 6000, 1, 0.1f, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR);

//         if (i % 100 == 0)
//             printf("Loss: %.20f\n", SW_CalculateLoss(&network, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, ImageData[TestImageID], CorrectOutput[TestImageID]));
//     }

//     // Find which neuron was the strongest on the last layer
//     float LargestWeight = -1.0f;
//     uint32_t LargestWeightValue;

//     for (uint32_t i = 0; i < network.layers[network.layerAmount - 1].neuronAmount; i++)
//     {
//         // Also a bit more testing output
//         printf("%.2f ", network.layers[network.layerAmount - 1].neurons[i].output);

//         if (network.layers[network.layerAmount - 1].neurons[i].output > LargestWeight)
//         {
//             LargestWeight = network.layers[network.layerAmount - 1].neurons[i].output;
//             LargestWeightValue = i;
//         }
//     }

//     printf("\nOutput value: %u\n", LargestWeightValue);

//     SW_SaveNetwork(&network, "savednetwork");

//     SW_UnloadNetwork(&network);

//     return 0;
// }

// #include <stdio.h>
// #include <time.h>

// #include "Swan.h"

// int main(void)
// {
//     srand(time(NULL));

//     SW_Network network;
//     SW_InitNetwork(&network, 2);
//     SW_AddNetworkLayer(&network, 8, SW_ACTIVATION_FUNCTION_TANH);
//     SW_AddNetworkLayer(&network, 8, SW_ACTIVATION_FUNCTION_TANH);
//     SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_SIGMOID);

//     SW_RandomizeNetwork(&network);

//     SWM_Matrix inputMatrix;
//     SWM_initMatrix(&inputMatrix, 1, 2);
//     SWM_set(&inputMatrix, 0, 0, 0.5f);
//     SWM_set(&inputMatrix, 0, 1, 0.25f);
//     // SWM_set(&inputMatrix, 0, 2, 0.112f);
//     // SWM_set(&inputMatrix, 0, 3, 0.22f);

//     SWM_Matrix out = SW_ExecuteNetwork(&network, &inputMatrix);

//     SWM_printm(&out);

//     SW_UnloadNetwork(&network);

//     SWM_destroyMatrix(&out);
//     SWM_destroyMatrix(&inputMatrix);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <Swan.h>

#define DEBUG

// https://codereview.stackexchange.com/questions/151049/endianness-conversion-in-c
static inline uint32_t reverse32(uint32_t value) 
{
    return (((value & 0x000000FF) << 24) | ((value & 0x0000FF00) <<  8) | ((value & 0x00FF0000) >>  8) | ((value & 0xFF000000) >> 24));
}

static inline uint32_t freadReverse32(FILE *stream)
{
    uint32_t val;
    fread(&val, sizeof(uint32_t), 1, stream);
    val = reverse32(val);
    return val;
}


const char lightValues[10] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };

void printImageFromPointer(uint8_t *pointer, int32_t rows, int32_t columns)
{
    char output[rows*columns+rows+16];

    uint32_t index = 0, outIndex = 0;

    for (uint32_t i = 0; i < rows; i++)
    {
        for (uint32_t j = 0; j < columns; j++)
        {
            uint8_t lightval = (uint8_t)round((float)((float)pointer[index++]/(float)UINT8_MAX) * 10.0f);
            if (lightval > 9) lightval = 9;
            output[outIndex++] = lightValues[lightval];
        }
        output[outIndex++] = '\n';
    }
    output[outIndex] = '\0';
    fputs(output, stdout);
}


int main(void)
{
    srand(time(NULL));

    int32_t nTrainingLabels = 0, nTrainingImages = 0, nTestLabels = 0, nTestImages = 0;
    uint8_t *trainingLabels = NULL, *trainingImages = NULL, *testLabels = NULL, *testImages = NULL;

    // parsing the MNIST dataset
    // ------------------------------------------------



    // training labels
    FILE *fTrainingLabels = fopen("MNISTdataset/train-labels-idx1-ubyte", "rb");

    if (fTrainingLabels == NULL)
    {
        fputs("error opening training label file\n", stderr);
        abort();
    }

    int32_t trainingLabelsMagicBytes;

    trainingLabelsMagicBytes = (int32_t)freadReverse32(fTrainingLabels);
    nTrainingLabels = (int32_t)freadReverse32(fTrainingLabels);

    if (trainingLabelsMagicBytes != 2049 && trainingLabelsMagicBytes != 17301504)
    {
        fputs("training labels corrupted\n", stderr);
        abort();
    }

    trainingLabels = realloc(trainingLabels, sizeof(uint8_t) * nTrainingLabels);
    size_t trainingLabelsNRead = fread(trainingLabels, sizeof(uint8_t), nTrainingLabels, fTrainingLabels);
    
#ifdef DEBUG
    printf("read %zu bytes into trainingLabels\n", trainingLabelsNRead);
#endif

    fclose(fTrainingLabels);



    // training images
    FILE *fTrainingImages = fopen("MNISTdataset/train-images-idx3-ubyte", "rb");

    if (fTrainingImages == NULL)
    {
        fputs("error opening training images file\n", stderr);
        abort();
    }

    int32_t trainingImagesMagicBytes, trainingImagesRows, trainingImagesColumns;

    trainingImagesMagicBytes = (int32_t)freadReverse32(fTrainingImages);
    nTrainingImages = (int32_t)freadReverse32(fTrainingImages);
    trainingImagesRows = (int32_t)freadReverse32(fTrainingImages);
    trainingImagesColumns = (int32_t)freadReverse32(fTrainingImages);

    if (trainingImagesMagicBytes != 2051 && trainingImagesMagicBytes != 50855936)
    {
        fputs("training images corrupted\n", stderr);
        abort();
    }

    trainingImages = realloc(trainingImages, sizeof(uint8_t) * nTrainingImages * trainingImagesRows * trainingImagesColumns);
    size_t trainingImagesNRead = fread(trainingImages, sizeof(uint8_t), nTrainingImages * trainingImagesRows * trainingImagesColumns, fTrainingImages);

#ifdef DEBUG
    printf("read %zu bytes into trainingImages\n", trainingImagesNRead);
#endif

    fclose(fTrainingImages);



    // test labels
    FILE *fTestLabels = fopen("MNISTdataset/t10k-labels-idx1-ubyte", "rb");

    if (fTestLabels == NULL)
    {
        fputs("error opening test label file\n", stderr);
        abort();
    }

    int32_t testLabelsMagicBytes;

    testLabelsMagicBytes = (int32_t)freadReverse32(fTestLabels);
    nTestLabels = (int32_t)freadReverse32(fTestLabels);

    if (testLabelsMagicBytes != 2049 && testLabelsMagicBytes != 17301504)
    {
        fputs("test labels corrupted\n", stderr);
        abort();
    }

    testLabels = realloc(testLabels, sizeof(uint8_t) * nTestLabels);
    size_t testLabelsNRead = fread(testLabels, sizeof(uint8_t), nTestLabels, fTestLabels);

#ifdef DEBUG
    printf("read %zu bytes into testLabels\n", testLabelsNRead);
#endif

    fclose(fTestLabels);



    // test images
    FILE *fTestImages = fopen("MNISTdataset/t10k-images-idx3-ubyte", "rb");

    if (fTestImages == NULL)
    {
        fputs("error opening test images file\n", stderr);
        abort();
    }

    int32_t testImageMagicBytes, testImagesRows, testImagesColumns;

    testImageMagicBytes = (int32_t)freadReverse32(fTestImages);
    nTestImages = (int32_t)freadReverse32(fTestImages);
    testImagesRows = (int32_t)freadReverse32(fTestImages);
    testImagesColumns = (int32_t)freadReverse32(fTestImages);

    if (testImageMagicBytes != 2051 && testImageMagicBytes != 50855936)
    {
        fputs("test images corrupted\n", stderr);
        abort();
    }

    testImages = realloc(testImages, sizeof(uint8_t) * nTestImages * testImagesRows * testImagesColumns);
    size_t testImagesNRead = fread(testImages, sizeof(uint8_t), nTestImages * testImagesRows * testImagesColumns, fTestImages);

#ifdef DEBUG
    printf("read %zu bytes into testImages\n", testImagesNRead);
#endif


    fclose(fTestImages);



    
    // ------------------------------------------------

#ifdef DEBUG

    printf("First 2 images of training set:\n");
    uint8_t *trainingpointer = trainingImages;
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", trainingLabels[i]);
        printImageFromPointer(trainingpointer, 28, 28);
        trainingpointer += (28 * 28);
    }

    printf("First 2 images of test set:\n");
    uint8_t *testpointer = testImages;
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", testLabels[i]);
        printImageFromPointer(testpointer, 28, 28);
        testpointer += (28 * 28);
    }
    
#endif


    free(testImages);
    free(trainingImages);
    free(testLabels);
    free(trainingLabels);

    return 0;
}