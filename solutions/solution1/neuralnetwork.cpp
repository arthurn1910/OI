#include "neuralnetwork.h"
#include <iostream>
#include <cmath>
#include <QDebug>

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

    int lastLayerSize = layers[layerCount - 1]->getLayerSize();
    double **output = new double*[lastLayerSize];

    for (int i = 0; i < lastLayerSize; i += 2) {
        output[i / 2] = new double[2];
        output[i / 2][0] = currentInput[i];
        output[i / 2][1] = currentInput[i+1];
    }

    return output;
}
