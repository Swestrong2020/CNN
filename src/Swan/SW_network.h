#ifndef SW_NETWORK_H
#define SW_NETWORK_H

#include "SW_types.h"

void SW_InitNetwork(SW_Network *network);
void SW_AddNetworkLayer(SW_Network *network, unsigned int neuronAmount, SW_ActivationFunction activationFunction);

void SW_RandomizeNetwork(SW_Network *network);

void SW_SetNetworkInput(SW_Network *network, float *input);   // input should have the same length as the first layer in the network

void SW_TrainGradientDescent(SW_Network *network, float **input, unsigned int inputAmount, unsigned int batchSize, float minimumLoss); // input should be an array of length inputAmount, containing arrays of the length of the first layer of the neural network
void SW_ExucuteNetwork(SW_Network *network);

float SW_CalculateLoss(SW_Network *network, SW_LossFunction lossFunction, float *input, float *correctOutput); // input should have the same length as the first layer in the network, and correctOutput should have the same length as the last layer in the network


#endif // SW_NETWORK_H
