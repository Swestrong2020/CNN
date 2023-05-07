#ifndef NETWORK_H
#define NETWORK_H

#include "types.h"

void InitNetwork(Network *network);
void AddNetworkLayer(Network *network, unsigned int neuronAmount);
void RandomizeNetwork(Network *network);

#endif // NETWORK_H
