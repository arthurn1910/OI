#include "neuralnetworklayer.h"
#include <QDebug>

NeuralNetworkLayer::NeuralNetworkLayer(pugi::xml_node layerNode)
{
    layerSize = std::atoi(layerNode.attribute("neuronCount").value());
    neurons = new Neuron*[layerSize];

    int currentNeuron = 0;
    for (pugi::xml_node neuron = layerNode.first_child(); neuron; neuron = neuron.next_sibling())
    {
        neurons[currentNeuron++] = new Neuron(neuron);
    }
}

int NeuralNetworkLayer::getLayerSize()
{
    return layerSize;
}


double *NeuralNetworkLayer::process(double *inputData)
{
    double *output = new double[layerSize];

    for (int i = 0; i < layerSize; i++) {
        output[i] = neurons[i]->process(inputData);
    }

    delete [] inputData;

    return output;
}
