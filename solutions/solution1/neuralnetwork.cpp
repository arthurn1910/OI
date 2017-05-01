#include "neuralnetwork.h"
#include <iostream>
#include <cmath>

NeuralNetwork::NeuralNetwork(pugi::xml_document *document)
{
    pugi::xml_node neuralNetworkNode = document->child("NeuralNetwork");
    layerCount = std::atoi(neuralNetworkNode.attribute("layerCount").value());
    layers = new NeuralNetworkLayer*[layerCount];

    int currentLayer = 0;
    for (pugi::xml_node neuralNetworkLayer = neuralNetworkNode.first_child(); neuralNetworkLayer; neuralNetworkLayer = neuralNetworkLayer.next_sibling())
    {
        layers[currentLayer++] = new NeuralNetworkLayer(neuralNetworkLayer);
    }
}

NeuralNetwork::~NeuralNetwork()
{
    for (int i = 0; i < layerCount; i++) {
        delete layers[i];
    }
    delete [] layers;
}


double **NeuralNetwork::process(double *input)
{
    double *currentInput = input;
    for (int i = 0; i < layerCount; i++) {
        currentInput = layers[i]->process(currentInput);
    }

    int lastLayerSizeHalved = layers[layerCount - 1]->getLayerSize() / 2;
    double **output = new double*[lastLayerSizeHalved];

    for (int i = 0; i < lastLayerSizeHalved; i += 2) {
        output[i] = new double[2] { currentInput[i], currentInput[i+1] };
    }

    return output;
}
