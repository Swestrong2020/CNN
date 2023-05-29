#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Swan/Swan.h"

int main(void)
{
    uint32_t TestImageID = 0;

    // Load the MNIST dataset
    // Some of this is a bit of a mess, because I didn't want to take the effort to fix big and little endianness issues
    FILE *MNISTLabelFile = fopen("MNISTdataset/train-labels-idx1-ubyte", "rb");

    int32_t MNISTLabelsMagicBytes;
    fread(&MNISTLabelsMagicBytes, sizeof(int32_t), 1, MNISTLabelFile);

    // Sure read unnecesary data into some temp value nothing can go wrong with that (fseek was too hard)
    float TEMP;

    // Magic mumbers are different from the ones provided by the dataset, because of endianness stupidness
    if (MNISTLabelsMagicBytes != 17301504 && MNISTLabelsMagicBytes != 2049)
    {
        puts("Looks like something's wrong with the provided MNIST dataset (label)");
        printf("%i", MNISTLabelsMagicBytes);
        return -1;
    }

    fread(&TEMP, sizeof(uint32_t), 1, MNISTLabelFile);

    uint8_t *MNISTLabels = malloc(sizeof(uint8_t) * 6000);

    fread(MNISTLabels, sizeof(uint8_t), 6000, MNISTLabelFile);

    fclose(MNISTLabelFile);

    FILE *MNISTImageFile = fopen("MNISTdataset/train-images-idx3-ubyte", "rb");

    int32_t MNISTImagesMagicBytes;
    fread(&MNISTImagesMagicBytes, sizeof(int32_t), 1, MNISTImageFile);

    if (MNISTImagesMagicBytes != 50855936 && MNISTImagesMagicBytes != 2051)
    {
        puts("Looks like something's wrong with the provided MNIST dataset (image)");
        return -1;
    }

    fread(&TEMP, sizeof(uint32_t), 1, MNISTImageFile);

    float **ImageData = malloc(sizeof(float *) * 6000);

    for (int32_t i = 0; i < 6000; i++)
    {
        fread(&TEMP, sizeof(int32_t), 1, MNISTImageFile);
        fread(&TEMP, sizeof(int32_t), 1, MNISTImageFile);

        ImageData[i] = malloc(sizeof(float) * 28 * 28);

        for (uint32_t j = 0; j < 28 * 28; j++)
        {
            uint8_t Val;
            fread(&Val, sizeof(uint8_t), 1, MNISTImageFile);
            ImageData[i][j] = Val / 256.0f;
        }
    }

    fclose(MNISTImageFile);

    // Debug, test if things loaded succesfully
    for (int32_t row = 0; row < 28; row++)
    {
        for (int32_t column = 0; column < 28; column++)
        {
            if (ImageData[TestImageID][row * 28 + column] > 0.66f)
                putchar('x');
            else if (ImageData[TestImageID][row * 28 + column] > 0.33f)
                putchar('-');
            else
                putchar(' ');
        }

        putchar('\n');
    }

    // Example network
    SW_Network network;

    SW_InitNetwork(&network);
    SW_AddNetworkLayer(&network, 28 * 28, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_SIGMOID);

    SW_RandomizeNetwork(&network);
    
    // SW_LoadNetwork(&network, "savednetwork");

    // Calculate all the correct outputs to train the network with
    float **CorrectOutput = malloc(sizeof(float *) * 6000);

    for (uint32_t i = 0; i < 6000; i++)
    {
        CorrectOutput[i] = calloc(10, sizeof(float));
        CorrectOutput[i][MNISTLabels[i]] = 1.0f;
    }

    // A testing loop that shows the loss every so often
    printf("Loss: %.20f\n", SW_CalculateLoss(&network, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, ImageData[TestImageID], CorrectOutput[TestImageID]));

    for (uint32_t i = 0; i < 5000; i++)
    {
        SW_TrainNeuralNetwork(&network, ImageData, CorrectOutput, 6000, 1, 0.1f, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR);

        if (i % 100 == 0)
            printf("Loss: %.20f\n", SW_CalculateLoss(&network, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, ImageData[TestImageID], CorrectOutput[TestImageID]));
    }

    // Find which neuron was the strongest on the last layer
    float LargestWeight = -1.0f;
    uint32_t LargestWeightValue;

    for (uint32_t i = 0; i < network.layers[network.layerAmount - 1].neuronAmount; i++)
    {
        // Also a bit more testing output
        printf("%.2f ", network.layers[network.layerAmount - 1].neurons[i].output);

        if (network.layers[network.layerAmount - 1].neurons[i].output > LargestWeight)
        {
            LargestWeight = network.layers[network.layerAmount - 1].neurons[i].output;
            LargestWeightValue = i;
        }
    }

    printf("\nOutput value: %u\n", LargestWeightValue);

    SW_SaveNetwork(&network, "savednetwork");

    SW_UnloadNetwork(&network);

    return 0;
}




// #include <stdio.h>

// #include "SW_matrix.h"

// int main(void)
// {
//     SWM_Matrix a;
//     SWM_initMatrix(&a, 2, 5);

//     SWM_set(&a, 0, 0, 6);
//     SWM_set(&a, 0, 1, 234);
//     SWM_set(&a, 0, 2, 2);
//     SWM_set(&a, 0, 3, .87);
//     SWM_set(&a, 0, 4, 11);

//     SWM_set(&a, 1, 0, 3413);
//     SWM_set(&a, 1, 1, 1);
//     SWM_set(&a, 1, 2, 55);
//     SWM_set(&a, 1, 3, 112.7);
//     SWM_set(&a, 1, 4, .03);

//     SWM_printm(&a);
//     fputs("\n", stdout);

//     SWM_destroyMatrix(&a);

//     return 0;
// }