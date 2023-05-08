#ifndef TYPES_H
#define TYPES_H

typedef struct Neuron
{
    float *weights; // The weights for each connection with a neurons in the previous layer
    float *biases;  // The biases for each connection with a neurons in the previous layer

    float output;   // Its output
} Neuron;

typedef enum ActivationFunction
{
    ACTIVATION_FUNCTION_RELU = 0,
    ACTIVATION_FUNCTION_SOFTMAX,
    ACTIVATION_FUNCTION_SIGMOID,
    ACTIVATION_FUNCTION_TANH
} ActivationFunction;

typedef struct Layer
{
    Neuron *neurons;

    unsigned int neuronAmount;

    ActivationFunction activationFunction;
} Layer;

typedef struct Network
{
    Layer *layers;

    unsigned int layerAmount;
} Network;

#endif // TYPES_H
