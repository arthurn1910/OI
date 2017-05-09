#include "neuron.h"
#include "forwardfunction.h"
#include "sigmoidfunction.h"
#include <cmath>
#include <QDebug>
#include <iostream>

Neuron::Neuron(pugi::xml_node neuronNode)
{
    int activationFunctionId = std::atoi(neuronNode.child("ActivationFunction").attribute("function").value());
    if (activationFunctionId == 1) {
        double param = std::atof(neuronNode.child("ActivationFunction").attribute("param").value());
        createActivationFunction(activationFunctionId, param);
    } else {
        createActivationFunction(activationFunctionId);
    }

    useBias = std::atoi(neuronNode.child("Bias").attribute("use").value());
    biasWeight = std::atof(neuronNode.child("Bias").attribute("weight").value());

    pugi::xml_node inputsNode = neuronNode.child("Inputs");
    numberOfInputs = std::atoi(inputsNode.attribute("inputCount").value());
    weights = new double[numberOfInputs];

    pugi::xml_node inputWeight = inputsNode.child("InputWeight");
    for (int i = 0; i < numberOfInputs; i++) {
        weights[i] = std::atof(inputWeight.attribute("weight").value());
        inputWeight = inputWeight.next_sibling();
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
        output += weights[i] * input[i];
        //qDebug() << "\t\tPart out " << weights[i] << " * " << input[i];
    }

    //qDebug() << "\t\tBef bias " << output;
    output += useBias * biasWeight;

    //qDebug() << "\t\tBef activ " << output;
    output = activation->process(output);

    //qDebug() << "\t\tAfter activ " << output;
    return output;
}


void Neuron::createActivationFunction(int id)
{
    switch (id) {
    case 0:
        activation = new ForwardFunction();
        break;
    case 1:
        activation = new SigmoidFunction();
        break;
    default:
        throw new std::exception();
        break;
    }
}

void Neuron::createActivationFunction(int id, double param)
{
    switch (id) {
    case 0:
        activation = new ForwardFunction();
        break;
    case 1:
        activation = new SigmoidFunction(param);
        break;
    default:
        throw new std::exception();
        break;
    }
}
