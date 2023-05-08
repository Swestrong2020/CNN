#include "network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "types.h"

void InitNetwork(Network *network)
{
    network->layers = malloc(0);
    network->layerAmount = 0;
}

void AddNetworkLayer(Network *network, unsigned int neuronAmount, ActivationFunction activationFunction)
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

    CurrentLayer->activationFunction = activationFunction;

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
    srand(time(NULL));

    for (unsigned int i = 1; i < network->layerAmount; i++)
        for (unsigned int j = 0; j < network->layers[i].neuronAmount; j++)
            for (unsigned int k = 0; k < network->layers[i - 1].neuronAmount; k++)
            {
                network->layers[i].neurons[j].weights[k] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
                network->layers[i].neurons[j].biases[k]  = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            }
}

void SetNetworkInput(Network *network, float *data, unsigned int dataAmoun)
{
// unimplemented
}

void ExucuteNetwork(Network *network)
{
    if (network->layerAmount <= 2)
    {
        fputs("You can't execute a network without any layers, stupid", stderr);
        return;
    }

    // Calculate the output for each neuron in each layer
    for (unsigned int i = 1; i < network->layerAmount; i++)
    {
        Layer *PreviousLayer = &network->layers[i - 1];
        Layer *CurrentLayer = &network->layers[i];

        for (unsigned int j = 0; j < CurrentLayer->neuronAmount; j++)
        {
            float Input = 0.0f;

            for (unsigned int k = 0; k < PreviousLayer->neuronAmount; k++)
                Input += PreviousLayer->neurons[k].output * CurrentLayer->neurons[j].weights[k] + CurrentLayer->neurons[j].biases[k];
  
            // The activation function ReLU (Rectified linear)
            switch (CurrentLayer->activationFunction)
            {
            case ACTIVATION_FUNCTION_RELU:
                if (Input > 0.0f)
                    CurrentLayer->neurons[j].output = Input;
                else
                    CurrentLayer->neurons[j].output = 0.0f;
                break;

            case ACTIVATION_FUNCTION_SOFTMAX:
                // unimplemented
                break;

            case ACTIVATION_FUNCTION_SIGMOID:
                CurrentLayer->neurons[j].output = 1.0f / (1.0f + expf(-Input));
                break;

            case ACTIVATION_FUNCTION_TANH:
                CurrentLayer->neurons[j].output = tanh(Input) * 0.5f + 0.5f;
                break;

            default:
                puts("OH GOD YOU HAVE NO ACTIVATION FUNCTION WHAT HAVE YOU DONE");
                CurrentLayer->neurons[j].output = Input;
                break;
            }
        }
    }
}


float LossCalc(Network *network)
{
    
}