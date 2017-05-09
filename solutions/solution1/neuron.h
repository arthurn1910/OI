#ifndef NEURON_H
#define NEURON_H

#include "pugixml/pugixml.hpp"
#include "activationfunction.h"
#include "forwardfunction.h"

class Neuron
{
public:
    Neuron(pugi::xml_node neuronNode);
    ~Neuron();

    double process(double *inputs);

private:
    void createActivationFunction(int id);
    void createActivationFunction(int id, double param);
    int numberOfInputs;
    double *weights;
    double biasWeight;
    int useBias;
    ActivationFunction *activation;
};

#endif // NEURON_H
