#ifndef NEURALNETWORKLAYER_H
#define NEURALNETWORKLAYER_H

#include "neuron.h"
#include "pugixml/pugixml.hpp"

class NeuralNetworkLayer
{
public:
    NeuralNetworkLayer(pugi::xml_node layerNode);

    double *process(double *inputData);
    int getLayerSize();
private:
    int layerSize;
    Neuron **neurons;
};

#endif // NEURALNETWORKLAYER_H
