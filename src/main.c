#include <stdio.h>

#include "network.h"
#include "types.h"

int main(void)
{
    // Example network
    Network network;

    InitNetwork(&network);
    AddNetworkLayer(&network, 28 * 28, ACTIVATION_FUNCTION_RELU);
    AddNetworkLayer(&network, 32, ACTIVATION_FUNCTION_RELU);
    AddNetworkLayer(&network, 32, ACTIVATION_FUNCTION_RELU);
    AddNetworkLayer(&network, 10, ACTIVATION_FUNCTION_SIGMOID);

    RandomizeNetwork(&network);
    network.layers[0].neurons[0].output = 0.9f;
    network.layers[0].neurons[3].output = 0.8f;
    network.layers[0].neurons[7].output = 0.6f;
    network.layers[0].neurons[8].output = 0.5f;

    ExucuteNetwork(&network);

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

    return 0;
}
