#include "SW_network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "SW_types.h"
#include "SW_util.h"
#include "SW_matrix.h"
#include "SW_parse.h"

typedef struct SingleGradient
{

    SWM_Matrix weightGradient;
    SWM_Matrix biasGradient;

} SingleGradient;

typedef SingleGradient* NetworkGradients;

void initGradients(SW_Network *network, NetworkGradients *destGradient)
{
    (*destGradient) = malloc(sizeof(SingleGradient) * network->layerAmount);

    if ((*destGradient) == NULL)
    {
        fputs("error allocating network gradients\n", stderr);
        abort();
    }

    // initialize all matrices to store the gradients
    for (uint32_t i = 0; i < network->layerAmount; i++)
    {
        SWM_init(&destGradient[i]->biasGradient, network->layers[i].weights.rows, network->layers[i].weights.columns);
        SWM_init(&destGradient[i]->weightGradient, network->layers[i].biases.rows, network->layers[i].biases.columns);
    }
}

/* also pass the network because gradient size depends on network size*/
void destroyGradients(SW_Network *network, NetworkGradients *gradient)
{
    for (uint32_t i = 0; i < network->layerAmount; i++)
    {
        SWM_destroyMatrix(&gradient[i]->biasGradient);
        SWM_destroyMatrix(&gradient[i]->weightGradient);
    }
}

void SW_InitNetwork(SW_Network *network, uint32_t inputNeurons)
{
    network->layers = malloc(0);
    network->layerAmount = 0;
    network->inputNeurons = inputNeurons;
}

void SW_AddNetworkLayer(SW_Network *network, uint32_t neuronAmount, SW_ActivationFunction activationFunction)
{
    if (neuronAmount == 0)
    {
        SW_FATAL("WHAT'S THE POINT OF A NEURAL NETWORK IF IT LITERALLY HAS NO BRAIN? IS IT INSPIRED BY YOURSELF?\n")
    }

    uint32_t previouslayerNeurons = (network->layerAmount == 0) ? network->inputNeurons : network->layers[network->layerAmount-1].weights.columns;
    
    network->layers = realloc(network->layers, (++network->layerAmount) * sizeof(SW_Layer));
    if (network->layers == NULL)
    {
        SW_FATAL("Oopsie during layer allocation :(\n")
    }

    SW_Layer *currentLayer = &network->layers[network->layerAmount-1];

    currentLayer->activationFunction = activationFunction;

    // create a new matrix with neuronAmount rows, where each column holds weights for 
    // every neuron in the previous layer

    SWM_init(&currentLayer->weights, previouslayerNeurons, neuronAmount);
    SWM_init(&currentLayer->biases, 1, neuronAmount);

    SWM_fillMatrix(&currentLayer->weights, .0f);
    SWM_fillMatrix(&currentLayer->biases, .0f);
}

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

void applyActivation(SWM_Matrix *values, SW_ActivationFunction activationFunction)
{
    // apply activation function
    switch (activationFunction)
    {
        case SW_ACTIVATION_FUNCTION_RELU:
            SWM_applyFunction(values, &SW_ReLu);
            break;
        case SW_ACTIVATION_FUNCTION_SIGMOID:
            SWM_applyFunction(values, &SW_Sigmoid);
            break;
        case SW_ACTIVATION_FUNCTION_TANH:
            SWM_applyFunction(values, &SW_Tanh);
            break;
        case SW_ACTIVATION_FUNCTION_SOFTMAX:
            break;
    }
}

void SW_ExecuteNetwork(SW_Network *network, SWM_Matrix *input, SWM_Matrix *dest)
{
    if (network->layerAmount == 0)
    {
        SW_FATAL("Not executing empty network\n")
    }

    if (dest->rows != 1 || dest->columns != network->layers[network->layerAmount-1].weights.columns)
    {
        SW_FATAL("destination matrix should be of size (1, output neuron amount) where (row, column)\n")
    }

    // variables for storing the current values
    SW_Layer *currentLayer;
    SWM_Matrix currentOutput;

    // initialize the output with the size of the input
    SWM_init(&currentOutput, input->rows, input->columns);
    // copy the input data into the current output array
    memcpy(currentOutput.data, input->data, sizeof(SWM_MatrixValue_t) * input->rows * input->columns);

    // loop through every layer from left to right, multiply current output with the
    // weights, add bias and apply activation function
    for (int i = 0; i < network->layerAmount; i++)
    {
        currentLayer = &network->layers[i];

        SWM_Matrix tempOut = SWM_multiplyMatrix(&currentOutput, &currentLayer->weights);
        SWM_addMatrixD(&tempOut, &currentLayer->biases, &tempOut);

        applyActivation(&tempOut, currentLayer->activationFunction);

        // kinda jank, store the tempOut variable in the current output
        // have to free the previously allocated memory because the tempOut
        // might be of a different size than the current output matrix
        SWM_destroyMatrix(&currentOutput);  
        currentOutput = tempOut;
    }

    // return the current data
    memcpy(dest->data, currentOutput.data, sizeof(float) * currentOutput.rows * currentOutput.columns);
    SWM_destroyMatrix(&currentOutput);
}

/* calculates the gradient of the network with a single input/output pair */
void calculateGradient(SW_Network *network, SW_MNISTData_t *trainingData, SW_LossFunction lossFunction, uint32_t iInput /* the location of the input to use in the training data */, NetworkGradients *destGradient)
{
    // stochastic gradient descent implementation

    // offset the input pointer to the correct input image
    float *inputs = trainingData->images + (sizeof(float) * iInput * MNISTIMAGESIZE*MNISTIMAGESIZE);

    /* DON'T FREE THIS VARIABLE, it didn't allocate new memory, just points to existing memory */
    SWM_Matrix networkInputs;
    SWM_initData(&networkInputs, 1, MNISTIMAGESIZE*MNISTIMAGESIZE, inputs);

    // offset the output to point to the correct output label
    uint8_t correctOutputNumber = (*trainingData->labels + (sizeof(uint8_t) * iInput));
    
    uint32_t nOutputNeurons = network->layers[network->layerAmount-1].weights.columns;

    // convert correct output to a format that can be used by the loss function
    // specific to the mnist data set right now
    float correctOutput[nOutputNeurons];
    for (uint32_t i = 0; i < nOutputNeurons; i++)
        correctOutput[i] = 0;
    correctOutput[correctOutputNumber] = 1;

    // forward propagation while caching necessary values at every step
    SWM_Matrix 
        outputs[network->layerAmount],
        activations[network->layerAmount];

    SWM_Matrix *currentInput = &networkInputs;

    for (uint32_t i = 0; i < network->layerAmount; i++)
    {
        SW_Layer *cl = &network->layers[i];

        SWM_init(&outputs[i], 1, cl->weights.columns);
        SWM_init(&activations[i], 1, cl->weights.columns);

        // store the weighted input in outputs
        SWM_multiplyMatrixD(currentInput, &cl->weights, &outputs[i]);
        SWM_addMatrixD(&outputs[i], &cl->biases, &outputs[i]);

        // store activations in... activations
        memcpy(&activations[i].data, &outputs[i].data, sizeof(float) * outputs[i].rows * outputs[i].columns);
        applyActivation(&activations[i], cl->activationFunction);
    }





    // free cached values
    for (uint32_t i = 0; i < network->layerAmount; i++)
    {
        SWM_destroyMatrix(&outputs[i]);
        SWM_destroyMatrix(&activations[i]);
    }
}

void SW_TrainNetwork(SW_Network *network, SW_MNISTData_t *trainingData, uint32_t epochs, float learningRate, SW_LossFunction lossFunction, uint32_t batchSize)
{
    // loss should be calculated for entire batch, currently calculated for single image

    uint32_t currentInput = 0;

    NetworkGradients gradient;
    initGradients(network, &gradient);


    calculateGradient(network, trainingData, lossFunction, currentInput, &gradient);


    destroyGradients(network, &gradient);

    // // amount of output neurons
    // uint32_t nValues = network->layers[network->layerAmount-1].weights.columns;

    // // since outputCache->data is a row vector with one row, it can be treated as a float array
    // // because matrices are stored row-wise, so one can directly give the float pointer as an
    // // argument to the loss functions that operate on float arrays as opposed to matrices
    // float loss;

    // switch (lossFunction)
    // {
    //     case SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR:
    //         loss = SW_MeanSquaredError(networkOutputCache.data, ca, nValues);
    //         break;
    //     case SW_LOSS_FUNCTION_CROSS_ENTROPY:
    //         loss = SW_CrossEntropy(networkOutputCache.data, ca, nValues);
    //         break;
    //     default:
    //         SW_FATAL("not a cost function\n")
    //         break;
    // }

    // // got this from wikipedia, shit still leave me guessin' but getting there :/

    // // move backwards through the layers
    // for (uint32_t iLayer = network->layerAmount-1; iLayer >= 0; iLayer--)
    // {
         
    // }

}

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