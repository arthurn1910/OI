#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "neuron.h"
#include "activationfunction.h"
#include "pugixml/pugixml.hpp"

class NeuralNetwork {
public:
    int *networkVector;
    Neuron ***neurons;
    double **neuronsLastOutputs;

    NeuralNetwork(int *networkVector, double bias, double learningStep, double momentumStep, ActivationFunction **activation);

    double *process(double *input);

    double train(double *input, double *desiredOutput);

    double ***getNetworkState();

    pugi::xml_document *createXmlRepresentation();
};

#endif // NEURALNETWORK_H
