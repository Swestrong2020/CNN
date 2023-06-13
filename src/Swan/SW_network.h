#ifndef SW_NETWORK_H
#define SW_NETWORK_H

#include <stdint.h>

#include "SW_types.h"
#include "SW_matrix.h"
#include "SW_parse.h"

void SW_InitNetwork(SW_Network *network, uint32_t inputNeurons);
void SW_AddNetworkLayer(SW_Network *network, uint32_t neuronAmount, SW_ActivationFunction activationFunction);
void SW_UnloadNetwork(SW_Network *network);
void SW_RandomizeNetwork(SW_Network *network);

/* expects the destination matrix to be already initialized */
void SW_ExecuteNetwork(SW_Network *network, SWM_Matrix *input, SWM_Matrix *dest);
void SW_TrainNetwork(SW_Network *network, SW_MNISTData_t *trainingData, uint32_t epochs, float learningRate, SW_LossFunction lossFunction, uint32_t batchSize);

void SW_SaveNetwork(SW_Network *network, const char *fileName);
void SW_LoadNetwork(SW_Network *network, const char *fileName);

#endif // SW_NETWORK_H

