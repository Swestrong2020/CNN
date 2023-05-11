#include "SW_network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "SW_types.h"

void SW_InitNetwork(SW_Network *network)
{
    network->layers = malloc(0);
    network->layerAmount = 0;
}

void SW_AddNetworkLayer(SW_Network *network, unsigned int neuronAmount, SW_ActivationFunction activationFunction)
{
    if (neuronAmount == 0)
    {
        fputs("WHAT'S THE POINT OF A NEURAL NETWORK IF IT LITERALLY HAS NO BRAIN? IS IT INSPIRED BY YOURSELF?", stderr);
        return;
    }

    // Actually allocate the layer and its data
    network->layers = realloc(network->layers, (network->layerAmount + 1) * sizeof(SW_Layer));
    if (network->layers == NULL)
    {
        fputs("OH GOD OH CRAP EVERYTHING IS GOING WRONG", stderr);
        abort();
    }

    network->layerAmount++;

    SW_Layer *CurrentLayer = &network->layers[network->layerAmount - 1];

    CurrentLayer->neurons = malloc(sizeof(SW_Neuron) * neuronAmount);
    CurrentLayer->neuronAmount = neuronAmount;

    CurrentLayer->activationFunction = activationFunction;

    // Allocate the weights and biases for the neuron (if there is a previous layer to have those values for)
    if (network->layerAmount > 1)
    {
        unsigned int PreviousLayerNeuronAmount = network->layers[network->layerAmount - 2].neuronAmount;

        for (unsigned int i = 0; i < neuronAmount; i++)
        {
            CurrentLayer->neurons[i].weights = malloc(sizeof(float) * PreviousLayerNeuronAmount);
            memset(CurrentLayer->neurons[i].weights, 0, sizeof(float) * PreviousLayerNeuronAmount);

            CurrentLayer->neurons[i].bias = 0.0f;

            CurrentLayer->neurons[i].output = 0.0f;
        }
    }
}

void SW_UnloadNetwork(SW_Network *network)
{
    for (unsigned int i = 0; i < network->layerAmount; i++)
    {
        if (i > 0)
        {
            for (unsigned int j = 0; j < network->layers[i].neuronAmount; j++)
                free(network->layers[i].neurons[j].weights);
        }

        free(network->layers[i].neurons);
    }

    free(network->layers);
}

void SW_RandomizeNetwork(SW_Network *network)
{
    // Randomize all the weights and biases for each connection
    srand(time(NULL));

    for (unsigned int i = 1; i < network->layerAmount; i++)
        for (unsigned int j = 0; j < network->layers[i].neuronAmount; j++)
        {
            for (unsigned int k = 0; k < network->layers[i - 1].neuronAmount; k++)
                network->layers[i].neurons[j].weights[k] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;

            network->layers[i].neurons[j].bias = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        }
}

void SW_SetNetworkInput(SW_Network *network, float *input)
{
    if (network->layerAmount == 0)
    {
        fputs("It's a bit hard to give some input to a completely empty network, isn't it?", stderr);
        return;
    }

    if (input == NULL)
    {
        fputs("Would be a shame if the input data for the neural network got corrupted and not allocated, wouldn't it? Oh wait, that's the case.", stderr);
        return;
    }

    for (unsigned int i = 0; i < network->layers[0].neuronAmount; i++)
        network->layers[0].neurons[i].output = input[i];
}

void SW_TrainNeuralNetwork(SW_Network *network, float **input, float **correctOutput, unsigned int dataAmount, unsigned int batchSize, float targetLoss, SW_LossFunction lossFunction)
{
    // Let's start simple with only the core of the algorithm for now (back propagation)
    // That core of the algorithm is taking just one input and its label, and adjusting the weights and biases for that one case
  
// Not finished yet
    unsigned int testid = 0;
    SW_SetNetworkInput(network, input[testid]);
    SW_ExucuteNetwork(network);

    

}

void SW_ExucuteNetwork(SW_Network *network)
{
    if (network->layerAmount <= 2)
    {
        fputs("You can't execute a network without any layers, stupid", stderr);
        return;
    }

    // Calculate the output for each neuron in each layer
    for (unsigned int i = 1; i < network->layerAmount; i++)
    {
        SW_Layer *PreviousLayer = &network->layers[i - 1];
        SW_Layer *CurrentLayer = &network->layers[i];

        for (unsigned int j = 0; j < CurrentLayer->neuronAmount; j++)
        {
            float Input = 0.0f;

            for (unsigned int k = 0; k < PreviousLayer->neuronAmount; k++)
                Input += PreviousLayer->neurons[k].output * CurrentLayer->neurons[j].weights[k];
            
            Input += CurrentLayer->neurons[j].bias;
  
            // The activation function ReLU (Rectified linear)
            switch (CurrentLayer->activationFunction)
            {
            case SW_ACTIVATION_FUNCTION_RELU:
                if (Input > 0.0f)
                    CurrentLayer->neurons[j].output = Input;
                else
                    CurrentLayer->neurons[j].output = 0.0f;
                break;

            case SW_ACTIVATION_FUNCTION_SOFTMAX:
                // unimplemented
                break;

            case SW_ACTIVATION_FUNCTION_SIGMOID:
                CurrentLayer->neurons[j].output = 1.0f / (1.0f + expf(-Input));
                break;

            case SW_ACTIVATION_FUNCTION_TANH:
                CurrentLayer->neurons[j].output = tanh(Input) * 0.5f + 0.5f;
                break;

            default:
                fputs("OH GOD YOU HAVE NO ACTIVATION FUNCTION WHAT HAVE YOU DONE", stderr);
                CurrentLayer->neurons[j].output = Input;
                break;
            }
        }
    }
}

float SW_CalculateLoss(SW_Network *network, SW_LossFunction lossFunction, float *input, float *correctOutput)
{      
    SW_SetNetworkInput(network, input);
    SW_ExucuteNetwork(network);

    SW_Layer *LastLayer = &network->layers[network->layerAmount - 1];

    float Result;

    switch (lossFunction)
    {
    case SW_LOSS_FUNCTION_CROSS_ENTROPY:
        Result = 0.0f;

        for (unsigned int i = 0; i < LastLayer->neuronAmount; i++)
            // Log is undefined at 0, so there's a bit of extra logic making sure the input doesn't go that low
            if (LastLayer->neurons[i].output < 0.000001f)
                Result -= correctOutput[i] * logf(0.0001f);
            else
                Result -= correctOutput[i] * logf(LastLayer->neurons[i].output);
        break;

    case SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR:
        Result = 0.0f;

        for (unsigned int i = 0; i < LastLayer->neuronAmount; i++)
            Result += (correctOutput[i] - LastLayer->neurons[i].output) * (correctOutput[i] - LastLayer->neurons[i].output);

        Result /= LastLayer->neuronAmount;
        break;

    default:
        fputs("That's not really a loss function...", stderr);
        Result = 0.0f;
        break;
    }

    return Result;
}
