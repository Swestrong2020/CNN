#ifndef NETWORK_H
#define NETWORK_H

#include "types.h"

void InitNetwork(Network *network);
void AddNetworkLayer(Network *network, unsigned int neuronAmount, ActivationFunction activationFunction);
void RandomizeNetwork(Network *network);
float LossCalc(Network *network)

void ExucuteNetwork(Network *network);

#endif // NETWORK_H
