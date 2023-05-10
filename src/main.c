#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Swan/Swan.h"

int main(void)
{
    // Load the MNIST dataset
    // Some of this is a bit of a mess, because I didn't want to take the effort to fix big and little endianness issues
    FILE *MNISTLabelFile = fopen("src/MNISTdataset/train-labels-idx1-ubyte", "rb");

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

    FILE *MNISTImageFile = fopen("src/MNISTdataset/train-images-idx3-ubyte", "rb");

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

        for (unsigned int j = 0; j < 28 * 28; j++)
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
            if (ImageData[0][row * 28 + column] > 0.66f)
                putchar('x');
            else if (ImageData[0][row * 28 + column] > 0.33f)
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
    
    SW_SetNetworkInput(&network, ImageData[0]);

    SW_ExucuteNetwork(&network);

    // Debugging output
    for (unsigned int i = 0; i < network.layerAmount; i++)
    {
        printf("%i: ", network.layers[i].activationFunction);

        for (unsigned int j = 0; j < network.layers[i].neuronAmount; j++)
            printf("%.2f  ", network.layers[i].neurons[j].output);

        putchar('\n');
    }

    // Find which neuron was the strongest on the last layer
    float LargestWeight = -1.0f;
    unsigned int LargestWeightValue;

    for (unsigned int i = 0; i < network.layers[network.layerAmount - 1].neuronAmount; i++)
    {
        if (network.layers[network.layerAmount - 1].neurons[i].output > LargestWeight)
        {
            LargestWeight = network.layers[network.layerAmount - 1].neurons[i].output;
            LargestWeightValue = i;
        }
    }

    printf("Output value: %u\n", LargestWeightValue);


    // Calculate the loss
    unsigned int TestImageID = 0;

    float *CorrectOutput = malloc(sizeof(float) * 10);
    
    for (unsigned int i = 0; i < 10; i++)
        CorrectOutput[i] = 0.0f;

    CorrectOutput[MNISTLabels[TestImageID]] = 1.0f;

    printf("Loss: %f\n", SW_CalculateLoss(&network, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, ImageData[TestImageID], CorrectOutput));

    return 0;
}
