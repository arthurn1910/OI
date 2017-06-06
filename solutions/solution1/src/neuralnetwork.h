#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "pugixml/pugixml.hpp"
#include "neuralnetworklayer.h"

class NeuralNetwork
{
public:
    NeuralNetwork(pugi::xml_document *document);
    ~NeuralNetwork();

    double **process(double *input);
private:
    NeuralNetworkLayer **layers;
    int layerCount;
};

#endif // NEURALNETWORK_H
