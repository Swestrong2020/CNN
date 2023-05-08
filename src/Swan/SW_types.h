#ifndef SW_TYPES_H
#define SW_TYPES_H

typedef struct SW_Neuron
{
    float *weights; // The weights for each connection with a neurons in the previous layer
    float *biases;  // The biases for each connection with a neurons in the previous layer

    float output;   // Its output
} SW_Neuron;

typedef enum SW_ActivationFunction
{
    SW_ACTIVATION_FUNCTION_RELU = 0,
    SW_ACTIVATION_FUNCTION_SOFTMAX,
    SW_ACTIVATION_FUNCTION_SIGMOID,
    SW_ACTIVATION_FUNCTION_TANH
} SW_ActivationFunction;

typedef struct SW_Layer
{
    SW_Neuron *neurons;

    unsigned int neuronAmount;

    SW_ActivationFunction activationFunction;
} SW_Layer;

typedef struct SW_Network
{
    SW_Layer *layers;

    unsigned int layerAmount;
} SW_Network;

#endif // SW_TYPES_H