#ifndef SW_NETWORK_H
#define SW_NETWORK_H

#include "SW_types.h"

void SW_InitNetwork(SW_Network *network);
void SW_AddNetworkLayer(SW_Network *network, unsigned int neuronAmount, SW_ActivationFunction activationFunction);

void SW_RandomizeNetwork(SW_Network *network);

void SW_ExucuteNetwork(SW_Network *network);
void SW_BasicLossCalc(SW_Network *network);

#endif // SW_NETWORK_H
