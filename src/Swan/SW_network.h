#ifndef SW_NETWORK_H
#define SW_NETWORK_H

#include "SW_types.h"

void SW_InitNetwork(SW_Network *network);
void SW_AddNetworkLayer(SW_Network *network, unsigned int neuronAmount, SW_ActivationFunction activationFunction);
void SW_UnloadNetwork(SW_Network *network);

void SW_RandomizeNetwork(SW_Network *network);

void SW_SetNetworkInput(SW_Network *network, float *input);   // input should have the same length as the first layer in the network

void SW_TrainNeuralNetwork(SW_Network *network, float **input, float **correctOutput, unsigned int dataAmount, unsigned int batchSize, float targetLoss, SW_LossFunction lossFunction); // input and correctOutput should be arrays of length dataAmount, each containing more arrays, for input of the size of the first layer, for correctOutput of the size of the last layer
void SW_ExucuteNetwork(SW_Network *network);
float SW_CalculateLoss(SW_Network *network, SW_LossFunction lossFunction, float *input, float *correctOutput); // input should have the same length as the first layer in the network, and correctOutput should have the same length as the last layer in the network


#endif // SW_NETWORK_H
