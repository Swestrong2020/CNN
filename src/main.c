#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include <Swan.h>

int main(void)
{
    srand(time(NULL));

    int32_t nTrainingLabels = 0, nTrainingImages = 0, nTestLabels = 0, nTestImages = 0;
    uint8_t *trainingLabels = NULL, *testLabels = NULL;
    float *testImages = NULL, *trainingImages = NULL;

    SW_parseMNIST(
        &nTrainingLabels, &nTrainingImages, &nTestLabels, &nTestImages,
        &trainingLabels, &trainingImages, &testLabels, &testImages
    );

    printf("First 2 images of training set:\n");
    float *trainingpointer = trainingImages;

    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", trainingLabels[i]);
        SW_printMNISTImage(trainingpointer);
        trainingpointer += (MNISTIMAGESIZE * MNISTIMAGESIZE);
    }

    printf("First 2 images of test set:\n");
    float *testpointer = testImages;
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", testLabels[i]);
        SW_printMNISTImage(testpointer);
        testpointer += (MNISTIMAGESIZE * MNISTIMAGESIZE);
    }



    // now only the neural network part :/

    SW_Network network;

    SW_InitNetwork(&network, MNISTIMAGESIZE*MNISTIMAGESIZE);
    SW_AddNetworkLayer(&network, 16, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 32, SW_ACTIVATION_FUNCTION_RELU);
    SW_AddNetworkLayer(&network, 10, SW_ACTIVATION_FUNCTION_RELU);

    SW_RandomizeNetwork(&network);

    // temp first image of training set
    SWM_Matrix inputMatrix = SW_MNISTImageToMatrix(trainingImages);

    SWM_Matrix outputMatrix = SW_ExecuteNetwork(&network, &inputMatrix);

    printf("test network output (with first image of training set):\n");
    SWM_printm(&outputMatrix);

    SWM_destroyMatrix(&inputMatrix);
    SWM_destroyMatrix(&outputMatrix);

    SW_UnloadNetwork(&network);

    free(testImages);
    free(trainingImages);
    free(testLabels);
    free(trainingLabels);

    return 0;
}