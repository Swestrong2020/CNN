#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

#include <Swan.h>

int main(void)
{
    srand(time(NULL));

    SW_MNISTData_t trainingData = {.images = NULL, .labels = NULL};
    SW_MNISTData_t testData = {.images = NULL, .labels = NULL};

    SW_parseMNIST(
        &trainingData, &testData
    );

    // now only the neural network part :/

    SW_Network network;

    SW_InitNetwork(&network, MNISTIMAGESIZE*MNISTIMAGESIZE);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_TANH);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_TANH);
    SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_SIGMOID);

    SW_RandomizeNetwork(&network);

    printf("first image of training set (should be %u):\n", trainingData.labels[0]);
    SW_printMNISTImage(trainingData.images);

    printf ("first image of test set (should be %u):\n", testData.labels[0]);
    SW_printMNISTImage(testData.images);

    SW_TrainNetwork(&network, &trainingData, 1, 1, SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR, 1);

    SW_UnloadNetwork(&network);

    SW_unloadMNIST(&trainingData);
    SW_unloadMNIST(&testData);

    return 0;
}