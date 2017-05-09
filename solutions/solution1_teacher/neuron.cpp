#include "neuron.h"
#include "forwardfunction.h"
#include "sigmoidfunction.h"
#include <algorithm>
#include <random>
#include <QDebug>

Neuron::Neuron(int n, double biasParam, double learningStep, double momentumStep)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-1, 1);

    this->inputs = n;
    this->lastSum = 0;
    this->delta = 0;
    this->activation = new ForwardFunction();
    this->learningStep = learningStep;
    this->weights = new double[inputs];
    this->momentumWeightChange = new double[inputs];
    this->momentumBiasChange = 0;
    this->momentumStep = momentumStep;

    for (int i = 0; i < inputs; i++)
        this->momentumWeightChange[i] = 0;

    // Losujemy wagi
    for (int i = 0; i < n; i++)
    {
        this->weights[i] = dist(e2);
    }
    this->biasWeight = dist(e2)*2-1;

    normalizeWeights();

    // 0 - brak bias, 1 - bias
    this->bias = biasParam;
}

double Neuron::feedForward(double *inputs)
{

    double value = 0;
    // Sumujemy wejścia * wagi
    for (int i = 0; i < this->inputs; i++)
    {
        value += weights[i] * inputs[i];
    }

    // Dodajemy bias jesli bias = 1
    value += biasWeight * bias;
    // Zapisujemy wartość
    lastSum = value;
    // Funkcja aktywacji
    return activation->activate(value);
}

void Neuron::learnDelta(double *inputs, double desired)
{
    // Obliczamy wartość dla wejścia
    double value = feedForward(inputs);

    // Nauka jeśli zły wynik
    if (value - desired != 0)
    {
        for (int i = 0; i < this->inputs; i++)
        {
            weights[i] += inputs[i] * learningStep * (desired - value);
        }

        biasWeight += learningStep * (desired - value);
    }
}

void Neuron::deltaUpdate(double desiredValue)
{
    delta = activation->derivativeActivate(lastSum) * (desiredValue - activation->activate(lastSum));
}

void Neuron::deltaUpdate(double *upperLayerDeltasTimesWeights, int upperLayerSize) {

    double sum = 0;

    for (int i = 0; i < upperLayerSize; i++)
    {
        sum += upperLayerDeltasTimesWeights[i];
    }

    delta = activation->derivativeActivate(lastSum) * sum;
}

void Neuron::weightsUpdate(double *input)
{
    double *oldWeights = new double[inputs];
    std::copy(weights, weights + inputs, oldWeights);

    double oldBiasWeight = biasWeight;

    for (int i = 0; i < inputs; i++)
    {
        weights[i] += learningStep * delta * input[i] + momentumStep * momentumWeightChange[i];
    }
    biasWeight += learningStep * delta + momentumStep * momentumBiasChange;

    normalizeWeights();

    for (int i = 0; i < inputs; i++)
    {
        momentumWeightChange[i] = weights[i] - oldWeights[i];
    }
    momentumBiasChange = biasWeight - oldBiasWeight;

    delete [] oldWeights;
}

double *Neuron::getWeights()
{
    double *weightsAndBias = new double[inputs + 1];

    for (int i = 0; i < inputs; i++)
    {
        weightsAndBias[i] = weights[i];
    }
    weightsAndBias[inputs] = biasWeight;

    return weightsAndBias;
}

void Neuron::normalizeWeights() {
    double maxWeight = -1;
    double minWeight = 1;
    for (int i = 0; i < inputs; i++) {
        if (weights[i] > maxWeight) {
            maxWeight = weights[i];
        }
        if (weights[i] < minWeight) {
            minWeight = weights[i];
        }
    }

    if (biasWeight > maxWeight) {
        maxWeight = biasWeight;
    }
    if (biasWeight < minWeight) {
        minWeight = biasWeight;
    }

    for (int i = 0; i < inputs; i++) {
        double old = weights[i];
        weights[i] = (2.0 / (maxWeight - minWeight)) * (weights[i] - minWeight) - 1;
    }
    biasWeight = (2 / (maxWeight - minWeight)) * (biasWeight - minWeight) - 1;
}

void Neuron::fillNeuronNode(pugi::xml_node &node)
{
    pugi::xml_node activationNode = node.append_child("ActivationFunction");
    if (ForwardFunction *f = dynamic_cast<ForwardFunction *>(activation))
        activationNode.append_attribute("function") = 0;
    else {
        SigmoidFunction *s = dynamic_cast<SigmoidFunction *>(activation);
        activationNode.append_attribute("function") = 1;
        activationNode.append_attribute("param") = s->beta;
    }

    pugi::xml_node biasNode = node.append_child("Bias");
    biasNode.append_attribute("use") = (int) bias;
    biasNode.append_attribute("weight") = biasWeight;

    pugi::xml_node inputsNode = node.append_child("Inputs");
    inputsNode.append_attribute("inputCount") = inputs;

    for (int i = 0; i < inputs; i++) {
        pugi::xml_node inputWeightNode = inputsNode.append_child("InputWeight");
        inputWeightNode.append_attribute("weight") = weights[i];
    }
}
