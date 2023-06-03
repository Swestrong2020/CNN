#include "SW_network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "SW_types.h"
#include "SW_util.h"
#include "SW_matrix.h"

void SW_InitNetwork(SW_Network *network, uint32_t inputNeurons)
{
    network->layers = malloc(0);
    network->layerAmount = 0;
    network->inputNeurons = inputNeurons;
}

// void SW_InitNetwork(SW_Network *network)
// {
//     network->layers = malloc(0);
//     network->layerAmount = 0;
// }

void SW_AddNetworkLayer(SW_Network *network, uint32_t neuronAmount, SW_ActivationFunction activationFunction)
{
    if (neuronAmount == 0)
    {
        fputs("WHAT'S THE POINT OF A NEURAL NETWORK IF IT LITERALLY HAS NO BRAIN? IS IT INSPIRED BY YOURSELF?", stderr);
        abort();
    }

    uint32_t previouslayerNeurons = (network->layerAmount == 0) ? network->inputNeurons : network->layers[network->layerAmount-1].weights.columns;
    
    network->layers = realloc(network->layers, (++network->layerAmount) * sizeof(SW_Layer));
    if (network->layers == NULL)
    {
        fputs("Oopsie during layer allocation :(\n", stderr);
        abort();
    }

    SW_Layer *currentLayer = &network->layers[network->layerAmount-1];

    currentLayer->activationFunction = activationFunction;

    // create a new matrix with neuronAmount rows, where each column holds weights for 
    // every neuron in the previous layer

    SWM_initMatrix(&currentLayer->weights, previouslayerNeurons, neuronAmount);
    SWM_initMatrix(&currentLayer->biases, 1, neuronAmount);

    SWM_fillMatrix(&currentLayer->weights, .0f);
    SWM_fillMatrix(&currentLayer->biases, .0f);
}

// void SW_AddNetworkLayer(SW_Network *network, uint32_t neuronAmount, SW_ActivationFunction activationFunction)
// {
    // if (neuronAmount == 0)
    // {
    //     fputs("WHAT'S THE POINT OF A NEURAL NETWORK IF IT LITERALLY HAS NO BRAIN? IS IT INSPIRED BY YOURSELF?", stderr);
    //     return;
    // }

//     // Actually allocate the layer and its data
//     network->layers = realloc(network->layers, (network->layerAmount + 1) * sizeof(SW_Layer));
//     if (network->layers == NULL)
//     {
//         fputs("OH GOD OH CRAP EVERYTHING IS GOING WRONG", stderr);
//         abort();
//     }

//     network->layerAmount++;

//     SW_Layer *CurrentLayer = &network->layers[network->layerAmount - 1];

//     CurrentLayer->neurons = malloc(sizeof(SW_Neuron) * neuronAmount);
//     CurrentLayer->neuronAmount = neuronAmount;

//     if (CurrentLayer->neurons == NULL)
//     {
//         fputs("Please get better RAM", stderr);
//         abort();
//     }

//     CurrentLayer->activationFunction = activationFunction;

//     // Allocate the weights and biases for the neuron (if there is a previous layer to have those values for)
//     if (network->layerAmount > 1)
//     {
//         uint32_t PreviousLayerNeuronAmount = network->layers[network->layerAmount - 2].neuronAmount;

//         for (uint32_t i = 0; i < neuronAmount; i++)
//         {
//             CurrentLayer->neurons[i].weights = malloc(sizeof(float) * PreviousLayerNeuronAmount);
//             if (CurrentLayer->neurons[i].weights == NULL)
//             {
//                 fputs("Everything is going wrong again!", stderr);
//                 abort();
//             }

//             memset(CurrentLayer->neurons[i].weights, 0, sizeof(float) * PreviousLayerNeuronAmount);

//             CurrentLayer->neurons[i].bias = 0.0f;

//             CurrentLayer->neurons[i].output = 0.0f;
//         }
//     }
// }

void SW_UnloadNetwork(SW_Network *network)
{
    for (uint32_t i = 0, li = network->layerAmount; i < li; i++)
    {
        SW_Layer *currentLayer = &network->layers[i];
        SWM_destroyMatrix(&currentLayer->biases);
        SWM_destroyMatrix(&currentLayer->weights);
    }
    free(network->layers);
}

// void SW_UnloadNetwork(SW_Network *network)
// {
//     for (uint32_t i = 0; i < network->layerAmount; i++)
//     {
//         if (i > 0)
//         {
//             for (uint32_t j = 0; j < network->layers[i].neuronAmount; j++)
//                 free(network->layers[i].neurons[j].weights);
//         }

//         free(network->layers[i].neurons);
//     }

//     free(network->layers);
// }

void SW_RandomizeNetwork(SW_Network *network)
{
    for (uint32_t i = 0, li = network->layerAmount; i < li; i++)
    {
        SW_Layer *currentLayer = &network->layers[i];

        for (uint32_t j = 0, lj = currentLayer->biases.rows * currentLayer->biases.columns; j < lj; j++)
        {
            currentLayer->biases.data[j] = SW_randf(-1, 1);
        }

        for (uint32_t j = 0, lj = currentLayer->weights.rows * currentLayer->weights.columns; j < lj; j++)
        {
            currentLayer->weights.data[j] = SW_randf(-1, 1);
        }
    }
}

// void SW_RandomizeNetwork(SW_Network *network)
// {
//     // Randomize all the weights and biases for each connection
//     srand(time(NULL));

//     for (uint32_t i = 1; i < network->layerAmount; i++)
//         for (uint32_t j = 0; j < network->layers[i].neuronAmount; j++)
//         {
//             for (uint32_t k = 0; k < network->layers[i - 1].neuronAmount; k++)
//                 network->layers[i].neurons[j].weights[k] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;

//             network->layers[i].neurons[j].bias = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
//         }
// }

// void SW_SetNetworkInput(SW_Network *network, float *input)
// {
//     if (network->layerAmount == 0)
//     {
//         fputs("It's a bit hard to give some input to a completely empty network, isn't it?", stderr);
//         return;
//     }

//     if (input == NULL)
//     {
//         fputs("Would be a shame if the input data for the neural network got corrupted and not allocated, wouldn't it? Oh wait, that's the case.", stderr);
//         return;
//     }

//     for (uint32_t i = 0; i < network->layers[0].neuronAmount; i++)
//         network->layers[0].neurons[i].output = input[i];
// }

// void SW_TrainNeuralNetwork(SW_Network *network, float **input, float **correctOutput, uint32_t dataAmount, uint32_t batchSize, float targetLoss, SW_LossFunction lossFunction)
// {
//     // Let's start simple with only the core of the algorithm for now (back propagation)
//     // That core of the algorithm is taking just one input and its label, and adjusting the weights and biases for that one case
  
//     // Not finished yet, for now it only works on one image at a time, and half the function arguments are ignored for now
//     // It doesn't seem to work fully yet... Also, it only works with mean squared error for now
//     uint32_t testid = 0;
//     float LearningRate  = 0.2f;

//     SW_SetNetworkInput(network, input[testid]);
//     SW_ExucuteNetwork(network);

//     // Loop backwards through all the layers
//     for (uint32_t i = network->layerAmount - 1; i > 0; i--)
//     {
//         SW_Layer *CurrentLayer = &network->layers[i];
//         SW_Layer *PreviousLayer = &network->layers[i - 1];
        
//         SW_Layer *NextLayer;

//         if (i < network->layerAmount - 1)
//             NextLayer = &network->layers[i + 1];

//         for (uint32_t j = 0; j < CurrentLayer->neuronAmount; j++)
//         {
//             // Begin with the part of the calculation thas is shared for each weight (how much the neuron influences the error, and the derivative of the activaion function at the output of the neuron)
//             float Error = 0.0f;

//             // The error is simple for the last layer, but it's a bit more complicated for any layer before that
//             if (i < network->layerAmount - 1)
//                 for (uint32_t h = 0; h < NextLayer->neuronAmount; h++)
//                     Error += NextLayer->neurons[h].error * NextLayer->neurons[h].activationDerivative * NextLayer->neurons[h].weights[j];
//             else
//                 Error = -(correctOutput[testid][j] - CurrentLayer->neurons[j].output);

//             // All the derivatives for the different activation functions
//             float ActivationDerivative = 0.0f;
  
//             switch (CurrentLayer->activationFunction)
//             {
//             case SW_ACTIVATION_FUNCTION_RELU:
//                 ActivationDerivative = SW_ReLu_Derivative(CurrentLayer->neurons[j].output);
//                 break;

//             case SW_ACTIVATION_FUNCTION_SOFTMAX:
//                 // unimplemented
//                 break;

//             case SW_ACTIVATION_FUNCTION_SIGMOID:
//                 ActivationDerivative = SW_Sigmoid_Derivative(CurrentLayer->neurons[j].output); 
//                 break;

//             case SW_ACTIVATION_FUNCTION_TANH:
//                 ActivationDerivative = SW_Tanh_Derivative(CurrentLayer->neurons[j].output);
//                 break;

//             default:
//                 fputs("Uh oh there's no activation function here", stderr);
//                 break;
//             }

//             // Also save these for the next (or well, previous) layer
//             CurrentLayer->neurons[j].error = Error;
//             CurrentLayer->neurons[j].activationDerivative = ActivationDerivative;

//             // Actually adjust all the weights using those values
//             for (uint32_t k = 0; k < PreviousLayer->neuronAmount; k++)
//             {
//                 // Calculate by how much to change the weights
//                 float Delta = Error * ActivationDerivative * PreviousLayer->neurons[k].output;
//                 CurrentLayer->neurons[j].weights[k] -= Delta * LearningRate;
//             }

//             // We can just treat the bias the same as a weight, but of which the previous neuron's output is always 1
//             float Delta = Error * ActivationDerivative;
//             CurrentLayer->neurons[j].bias -= Delta * LearningRate;
//         }
//     }
// }

SWM_Matrix SW_ExecuteNetwork(SW_Network *network, SWM_Matrix *input)
{
    if (network->layerAmount == 0)
    {
        fputs("Not executing empty network\n", stderr);
        abort();
    }

    SW_Layer *currentLayer;
    SWM_Matrix currentOutput;

    SWM_initMatrix(&currentOutput, input->rows, input->columns);
    memcpy(currentOutput.data, input->data, sizeof(SWM_MatrixValue_t) * input->rows * input->columns);

    for (int i = 0; i < network->layerAmount; i++)
    {
        currentLayer = &network->layers[i];

        SWM_Matrix tempOut = SWM_multiplyMatrix(&currentOutput, &currentLayer->weights);
        SWM_addMatrixD(&tempOut, &currentLayer->biases, &tempOut);

        // apply activation function
        switch (currentLayer->activationFunction)
        {
            case SW_ACTIVATION_FUNCTION_RELU:
                SWM_applyFunction(&tempOut, &SW_ReLu);
                break;
            case SW_ACTIVATION_FUNCTION_SIGMOID:
                SWM_applyFunction(&tempOut, &SW_Sigmoid);
                break;
            case SW_ACTIVATION_FUNCTION_TANH:
                SWM_applyFunction(&tempOut, &SW_Tanh);
                break;
            case SW_ACTIVATION_FUNCTION_SOFTMAX:
                break;
        }

        SWM_destroyMatrix(&currentOutput);
        currentOutput = tempOut;
    }

    return currentOutput;
}

// void SW_ExucuteNetwork(SW_Network *network)
// {
//     if (network->layerAmount <= 2)
//     {
//         fputs("You can't execute a network without any layers, stupid", stderr);
//         return;
//     }

//     // Calculate the output for each neuron in each layer
//     for (uint32_t i = 1; i < network->layerAmount; i++)
//     {
//         SW_Layer *PreviousLayer = &network->layers[i - 1];
//         SW_Layer *CurrentLayer = &network->layers[i];

//         for (uint32_t j = 0; j < CurrentLayer->neuronAmount; j++)
//         {
//             float input = 0.0f;

//             for (uint32_t k = 0; k < PreviousLayer->neuronAmount; k++)
//                 input += PreviousLayer->neurons[k].output * CurrentLayer->neurons[j].weights[k];
            
//             input += CurrentLayer->neurons[j].bias;
  
//             // The activation function ReLU (Rectified linear)
//             switch (CurrentLayer->activationFunction)
//             {
//             case SW_ACTIVATION_FUNCTION_RELU:
//                 CurrentLayer->neurons[j].output = SW_ReLu(input);
//                 break;

//             case SW_ACTIVATION_FUNCTION_SOFTMAX:
//                 // unimplemented
//                 break;

//             case SW_ACTIVATION_FUNCTION_SIGMOID:
//                 CurrentLayer->neurons[j].output = SW_Sigmoid(input);
//                 break;

//             case SW_ACTIVATION_FUNCTION_TANH:
//                 CurrentLayer->neurons[j].output = SW_Tanh(input);
//                 break;

//             default:
//                 fputs("OH GOD YOU HAVE NO ACTIVATION FUNCTION WHAT HAVE YOU DONE", stderr);
//                 CurrentLayer->neurons[j].output = 0.0f;
//                 break;
//             }
//         }
//     }
// }

// float SW_CalculateLoss(SW_Network *network, SW_LossFunction lossFunction, float *input, float *correctOutput)
// {      
//     SW_SetNetworkInput(network, input);
//     SW_ExucuteNetwork(network);

//     SW_Layer *LastLayer = &network->layers[network->layerAmount - 1];

//     float Result = 0.0f;

//     switch (lossFunction)
//     {
//     case SW_LOSS_FUNCTION_CROSS_ENTROPY:
//         for (uint32_t i = 0; i < LastLayer->neuronAmount; i++)
//             // Log is undefined at 0, so there's a bit of extra logic making sure the input doesn't go that low
//             if (LastLayer->neurons[i].output < 0.000001f)
//                 Result -= correctOutput[i] * logf(0.0001f);
//             else
//                 Result -= correctOutput[i] * logf(LastLayer->neurons[i].output);
//         break;

//     case SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR:
//         for (uint32_t i = 0; i < LastLayer->neuronAmount; i++)
//             Result += (correctOutput[i] - LastLayer->neurons[i].output) * (correctOutput[i] - LastLayer->neurons[i].output);

//         Result /= LastLayer->neuronAmount;
//         break;

//     default:
//         fputs("That's not really a loss function...", stderr);
//         break;
//     }

//     return Result;
// }

// void SW_SaveNetwork(SW_Network *network, char *fileName)
// {
//     FILE *File = fopen(fileName, "wb");

//     if (File == NULL)
//     {
//         fputs("Looks like your hard drive is dumb", stderr);
//         return;
//     }

//     fwrite(&network->layerAmount, sizeof(uint32_t), 1, File);

//     for (uint32_t i = 0; i < network->layerAmount; i++)
//     {
//         fwrite(&network->layers[i].activationFunction, sizeof(SW_LossFunction), 1, File);
//         fwrite(&network->layers[i].neuronAmount, sizeof(uint32_t), 1, File);
        
//         // neurons store connections to last layer, first layer is... the first, skip that
//         if (i == 0) continue;

//         for (uint32_t j = 0; j < network->layers[i].neuronAmount; j++)
//         {
//             fwrite(network->layers[i].neurons[j].weights, sizeof(float), network->layers[i - 1].neuronAmount, File);
//             fwrite(&network->layers[i].neurons[j].bias, sizeof(float), 1, File);
//         }
//     }

//     fclose(File);
// }

// /* fails if input network is already loaded */
// void SW_LoadNetwork(SW_Network *network, char *fileName)
// {
//     if (network->layerAmount)
//     {
//         fputs("Attempting to load into existing network, watch out, that shit's fatal cuh\n", stderr);
//         abort();
//     }

//     FILE *file = fopen(fileName, "rb");

//     if (file == NULL)
//     {
//         fputs("An oopsie happend with loading ur flies :(", stderr);
//         return;
//     }

//     uint32_t layerAmount;
//     fread(&layerAmount, sizeof(uint32_t), 1, file);

//     for (uint32_t i = 0; i < layerAmount; i++)
//     {
//         uint32_t activationFunction;
//         fread(&activationFunction, sizeof(SW_LossFunction), 1, file);

//         uint32_t neuronAmount;
//         fread(&neuronAmount, sizeof(uint32_t), 1, file);

//         SW_AddNetworkLayer(network, neuronAmount, activationFunction);

//         // neurons store connections to last layer, first layer is... the first, skip that
//         if (i == 0) continue;

//         for (uint32_t j = 0; j < neuronAmount; j++)
//         {
//             fread(network->layers[i].neurons[j].weights, sizeof(float), network->layers[i-1].neuronAmount, file);
//             fread(&(network->layers[i].neurons[j].bias), sizeof(float), 1, file);
//         }
//     }
    
//     fclose(file);
// }