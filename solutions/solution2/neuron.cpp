#include "neuron.h"
#include <QDebug>
#include <QString>

Neuron::Neuron(){

}

Neuron::Neuron(int neuronLayer, int numberOfInputs,
               int thisNeuronsNumberInLayer,int numberInputNeurons,
               int numberHiddenNeurons, int numberOutputNeurons)
{
    this->neuronLayer = neuronLayer;
    this->numberOfInputs = numberOfInputs;
    this->numberInLayer = thisNeuronsNumberInLayer;
    this->inputs = new double[numberOfInputs];
    this->weights = new double[numberOfInputs];
    this->previousWeights = new double[numberOfInputs];
    for (int i = 0; i < numberOfInputs; i++) {
        weights[i] = (rand() % 50)/(double)100;
    }
    this->numberHiddenNeurons=numberHiddenNeurons;
    this->numberInputNeurons=numberInputNeurons;
    this->numberOutputNeurons=numberOutputNeurons;
}

void Neuron:: changeWeights() {
    for (int i = 0; i < numberOfInputs; i++) {
            weights[i] = weights[i] + alpha * error * inputs[i];
            weights[i] = weights[i] + alpha * error * inputs[i];
    }
}

void Neuron::calculateError() {

        switch (neuronLayer) {
            case 0:
                break;
            case 1:
                calculateErrorForNeuronInHiddenLayer();
                break;
            case 2:
                calculateErrorForNeuronInOutputLayer();
                break;
        }
}

void Neuron::calculateOutput() {

    switch (this->neuronLayer) {
        case 0:
            calculateOutputForNeuronInInputLayer();
            break;
        case 1:
            calculateOutputForNeuronInHiddenLayer();
            break;
        case 2:
            calculateOutputForNeuronInOutputLayer();
            break;
    }
}

void Neuron::calculateOutputForNeuronInInputLayer() {
    output = inputs[0];
}

double Neuron::getOutput(){
    return this->output;
}

void Neuron::calculateOutputForNeuronInHiddenLayer() {
    sum = 0;

    for (int i = 0; i < numberOfInputs; i++) {
        sum += inputs[i] * weights[i];
    }

    sigmoidValue = sigmoid(sum, beta);
    output = sigmoidValue;
}

void Neuron::calculateOutputForNeuronInOutputLayer() {
    sum = 0;

    for (int i = 0; i < numberOfInputs; i++) {
        sum += inputs[i] * weights[i];
    }

    output = sum;
}

void Neuron::calculateErrorForNeuronInHiddenLayer() {
    double outputLayerSum = 0;
    for (int i=0; i<this->numberOutputNeurons;i++) {
        outputLayerSum += this->neuronsInLayerAbove[i].getWeights()[numberInLayer] * this->neuronsInLayerAbove[i].getError();
    }

    error = sigmoidDerivative(output) * outputLayerSum;
}

void Neuron::calculateErrorForNeuronInOutputLayer() {
    error = desiredValue - output;
}

double Neuron::sigmoid(double x, double beta) {
    return 1 / (1 + exp(-x * beta));
}

double Neuron::sigmoidDerivative(double sigm) {
    return sigm * (1 - sigm);
}

double* Neuron::getWeights() {
    return this->weights;
}

double Neuron::getError() {
    return this->error;
}

void Neuron::setInput(int inputNumber, double inputValue) {
    this->inputs[inputNumber] = inputValue;
}

void Neuron::setInputs(double* inputs) {
    this->inputs = inputs;
}

void Neuron::setOutNeurons(Neuron* neuronArray) {
    this->neuronsInLayerAbove = neuronArray;
}

void Neuron::setInNeurons(Neuron* neuronArray) {
    this->neuronsInLayerBelow = neuronArray;
}

int Neuron::getNumberOfInputs(){
    return this->numberOfInputs;
}

void Neuron::setDesiredValue(double desiredValue) {
    this->desiredValue = desiredValue;
}

Neuron* Neuron::getNeuronInLayerAbove(){
    return this->neuronsInLayerAbove;
}

Neuron* Neuron::getNeuronInLayerBelow(){
    return this->neuronsInLayerBelow;
}
