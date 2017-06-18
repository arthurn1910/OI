#ifndef NEURON_H
#define NEURON_H

#include <cstdlib>
#include <cmath>


class Neuron
{
public:
    Neuron();
    Neuron(int neuronLayer, int numberOfInputs,
           int thisNeuronsNumberInLayer, int numberInputNeurons,
           int numberHiddenNeurons, int numberOutputNeurons);
    void changeWeights();
    void calculateError();
    void calculateOutputForNeuronInHiddenLayer();
    void calculateOutputForNeuronInOutputLayer();
    void calculateErrorForNeuronInHiddenLayer();
    void calculateErrorForNeuronInOutputLayer();
    double sigmoid(double x, double beta);
    double sigmoidDerivative(double sigm);
    double* getWeights();
    double getError();
    void setInput(int inputNumber, double inputValue);
    void setInputs(double *inputs);
    void setOutNeurons(Neuron* neuronArray);
    void setInNeurons(Neuron* neuronArray);
    int getNumberOfInputs();
    void calculateOutput();
    void calculateOutputForNeuronInInputLayer();
    double getOutput();
    void setDesiredValue(double desiredValue);
    Neuron *getNeuronInLayerAbove();
    Neuron *getNeuronInLayerBelow();

private:
    double *weights;
    double *previousWeights;
    double *inputs;
    double sigmoidValue;
    Neuron *neuronsInLayerAbove;
    Neuron *neuronsInLayerBelow;
    double desiredValue;
    double error;
    double sum;
    double output;
    int neuronLayer;
    int numberOfInputs;
    int numberInLayer;
    double alpha = 0.003;
    double beta = 1;
    int numberInputNeurons;
    int numberHiddenNeurons;
    int numberOutputNeurons;




};

#endif // NEURON_H
