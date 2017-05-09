#ifndef NEURON_H
#define NEURON_H

#include "activationfunction.h"
#include "pugixml/pugixml.hpp"

class Neuron
{
public:
    int inputs;
    double lastSum;
    double delta;
    double learningStep;
    ActivationFunction *activation;

    double momentumStep;
    double *momentumWeightChange;
    double momentumBiasChange;
    double *weights;
    double biasWeight;
    double bias;

    Neuron(int n, double biasParam, double learningStep, double momentumStep);

    double feedForward(double *inputs);

    void learnDelta(double *inputs, double desired);

    void deltaUpdate(double desiredValue);

    void deltaUpdate(double *upperLayerDeltasTimesWeights, int upperLayerSize);

    void weightsUpdate(double *input);

    double *getWeights();

    void fillNeuronNode(pugi::xml_node &node);
private:
    void normalizeWeights();
};


#endif // NEURON_H
