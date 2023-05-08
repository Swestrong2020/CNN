#ifndef TYPES_H
#define TYPES_H

typedef struct Neuron
{
    float *weights; // The weights for each connection with a neurons in the previous layer
    float *biases;  // The biases for each connection with a neurons in the previous layer

    float output;   // Its output
} Neuron;

typedef struct Layer
{
    Neuron *neurons;

    unsigned int neuronAmount;
} Layer;

typedef struct Network
{
    Layer *layers;

    unsigned int layerAmount;
} Network;

#endif // TYPES_H
