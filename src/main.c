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
    uint8_t *trainingLabels = NULL, *trainingImages = NULL, *testLabels = NULL, *testImages = NULL;

    parseMNIST(
        &nTrainingLabels, &nTrainingImages, &nTestLabels, &nTestImages,
        &trainingLabels, &trainingImages, &testLabels, &testImages
    );

    printf("First 2 images of training set:\n");
    uint8_t *trainingpointer = trainingImages;
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", trainingLabels[i]);
        printMNISTImage(trainingpointer, 28, 28);
        trainingpointer += (28 * 28);
    }

    printf("First 2 images of test set:\n");
    uint8_t *testpointer = testImages;
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("should be: %d\n", testLabels[i]);
        printMNISTImage(testpointer, 28, 28);
        testpointer += (28 * 28);
    }



    // now only the neural network part :/3






    free(testImages);
    free(trainingImages);
    free(testLabels);
    free(trainingLabels);

    return 0;
}