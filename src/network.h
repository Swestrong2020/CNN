#ifndef NETWORK_H
#define NETWORK_H

#include "types.h"

void InitNetwork(Network *network);
void AddNetworkLayer(Network *network, unsigned int neuronAmount);
void RandomizeNetwork(Network *network);

void SetNetworkInput(Network *network, float *data, unsigned int dataAmount);
void ExucuteNetwork(Network *network);

// void LoadNetwork(Network *network);

#endif // NETWORK_H
