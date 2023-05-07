#include "network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.h"

void InitNetwork(Network *network)
{
    network->layers = malloc(0);
    network->layerAmount = 0;
}

void AddNetworkLayer(Network *network, unsigned int neuronAmount)
{
    if (neuronAmount == 0)
    {
        fputs("WHAT'S THE POINT OF A NEURAL NETWORK IF IT LITERALLY HAS NO BRAIN? IS IT INSPIRED BY YOURSELF?", stderr);
        return;
    }

    // Actually allocate the layer and its data
    network->layers = realloc(network->layers, (network->layerAmount + 1) * sizeof(Layer));
    if (network->layers == NULL)
    {
        fputs("OH GOD OH CRAP EVERYTHING IS GOING WRONG", stderr);
        abort();
    }

    network->layerAmount++;

    Layer *CurrentLayer = &network->layers[network->layerAmount - 1];

    CurrentLayer->neurons = malloc(sizeof(Neuron) * neuronAmount);
    CurrentLayer->neuronAmount = neuronAmount;

    // Allocate the weights and biases for the neuron (if there is a previous layer to have those values for)
    if (network->layerAmount > 1)
    {
        unsigned int PreviousLayerNeuronAmount = network->layers[network->layerAmount - 2].neuronAmount;

        for (unsigned int i = 0; i < neuronAmount; i++)
        {
            CurrentLayer->neurons[i].biases = malloc(sizeof(float) * PreviousLayerNeuronAmount);
            memset(CurrentLayer->neurons[i].biases, 0, sizeof(float) * PreviousLayerNeuronAmount);

            CurrentLayer->neurons[i].weights = malloc(sizeof(float) * PreviousLayerNeuronAmount);
            memset(CurrentLayer->neurons[i].weights, 0, sizeof(float) * PreviousLayerNeuronAmount);

            CurrentLayer->neurons[i].output = 0.0f;
        }
    }
}

void RandomizeNetwork(Network *network)
{

}

