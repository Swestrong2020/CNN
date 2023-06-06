#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

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
    SW_AddNetworkLayer(&network, 16, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_RELU);

    SW_RandomizeNetwork(&network);

    printf("first image of training set (should be %d):\n", trainingData.labels[0]);
    SW_printMNISTImage(trainingData.images);

    printf("first image of test set (should be %d):\n", testData.labels[0]);
    SW_printMNISTImage(testData.images);

    // temp first image of training set
    SWM_Matrix inputMatrix = SW_MNISTImageToMatrix(trainingData.images);

    SWM_Matrix outputMatrix = SW_ExecuteNetwork(&network, &inputMatrix);

    printf("test network output (with first image of training set):\n");
    SWM_printm(&outputMatrix);

    SWM_destroyMatrix(&inputMatrix);
    SWM_destroyMatrix(&outputMatrix);

    SW_UnloadNetwork(&network);

    SW_unloadMNIST(&trainingData);
    SW_unloadMNIST(&testData);

    return 0;
}