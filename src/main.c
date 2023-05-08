#include <stdio.h>

#include "network.h"
#include "types.h"

int main(void)
{
    // Example network
    Network network;

    InitNetwork(&network);
    AddNetworkLayer(&network, 16);
    AddNetworkLayer(&network, 8);
    AddNetworkLayer(&network, 8);
    AddNetworkLayer(&network, 2);

    RandomizeNetwork(&network);
    ExucuteNetwork(&network);

    // Debugging output
    for (unsigned int i = 0; i < network.layers[network.layerAmount - 1].neuronAmount; i++)
        printf("%f\n", network.layers[network.layerAmount - 1].neurons[i].output);

    return 0;
}
