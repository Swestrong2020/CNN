#ifndef SW_TYPES_H
#define SW_TYPES_H

#include <stdint.h>

#include "SW_matrix.h"

typedef enum SW_ActivationFunction
{
    SW_ACTIVATION_FUNCTION_RELU = 0,
    SW_ACTIVATION_FUNCTION_SOFTMAX,
    SW_ACTIVATION_FUNCTION_SIGMOID,
    SW_ACTIVATION_FUNCTION_TANH
} SW_ActivationFunction;

typedef enum SW_LossFunction
{
    SW_LOSS_FUNCTION_CROSS_ENTROPY = 0,
    SW_LOSS_FUNCTION_MEAN_SQUARED_ERROR
} SW_LossFunction;

typedef struct SW_Layer
{
    SWM_Matrix weights; /* matrix with weights for all neurons in the previous layer */
    SWM_Matrix biases; /* biases for every neuron in the current layer */

    SW_ActivationFunction activationFunction;
} SW_Layer;

typedef struct SW_Network
{
    SW_Layer *layers;
    uint32_t inputNeurons;

    uint32_t layerAmount;
} SW_Network;

#endif // SW_TYPES_H