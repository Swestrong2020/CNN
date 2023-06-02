#ifndef SW_NETWORK_H
#define SW_NETWORK_H

#include <stdint.h>

#include "SW_types.h"
#include "SW_matrix.h"

void SW_InitNetwork(SW_Network *network, uint32_t inputNeurons);
void SW_AddNetworkLayer(SW_Network *network, uint32_t neuronAmount, SW_ActivationFunction activationFunction);
void SW_UnloadNetwork(SW_Network *network);

void SW_RandomizeNetwork(SW_Network *network);

void SW_SetNetworkInput(SW_Network *network, float *input);   // input should have the same length as the first layer in the network

void SW_TrainNeuralNetwork(SW_Network *network, float **input, float **correctOutput, uint32_t dataAmount, uint32_t batchSize, float targetLoss, SW_LossFunction lossFunction); // input and correctOutput should be arrays of length dataAmount, each containing more arrays, for input of the size of the first layer, for correctOutput of the size of the last layer
void SW_ExecuteNetwork(SW_Network *network, SWM_Matrix *input);
float SW_CalculateLoss(SW_Network *network, SW_LossFunction lossFunction, float *input, float *correctOutput); // input should have the same length as the first layer in the network, and correctOutput should have the same length as the last layer in the network

void SW_SaveNetwork(SW_Network *network, char *fileName);
void SW_LoadNetwork(SW_Network *network, char *fileName);

#endif // SW_NETWORK_H

