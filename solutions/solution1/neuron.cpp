#include "neuron.h"

#include <cmath>

Neuron::Neuron(pugi::xml_node neuronNode)
{
    int activationFunctionId = std::atoi(neuronNode.child("ActivationFunction").attribute("function").value());
    createActivationFunction(activationFunctionId);

    useBias = std::atoi(neuronNode.child("Bias").attribute("use").value());
    biasWeight = std::atof(neuronNode.child("Bias").attribute("weight").value());

    pugi::xml_node inputsNode = neuronNode.child("Inputs");
    numberOfInputs = std::atoi(inputsNode.attribute("inputCount").value());
    weights = new double[numberOfInputs];

    int currentWeight = 0;
    for (pugi::xml_node inputWeight = inputsNode.first_child(); inputWeight; inputWeight = inputWeight.next_sibling()) {
        weights[currentWeight++] = std::atof(inputWeight.attribute("weight").value());
    }
}

Neuron::~Neuron()
{
    delete activation;
}


double Neuron::process(double *input)
{
    double output = 0;

    for (int i = 0; i < numberOfInputs; i++) {
        output += weights[i] * input[i] + useBias * biasWeight;
    }

    output = activation->process(output);

    return output;
}


void Neuron::createActivationFunction(int id)
{
    switch (id) {
    case 0:
        activation = new ForwardFunction();
        break;
    default:
        throw new std::exception();
        break;
    }
}
