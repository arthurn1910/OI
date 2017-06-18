#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <picture.h>
#include <QDebug>
#include <neuron.h>

#define SIZE 30
#define NETWORK_INPUT_LAYER SIZE * SIZE
#define EPOCHS 40
#define NETWORK_HIDDEN_LAYER 200
#define NETWORK_OUTPUT_LAYER 40
#define ALPHA 0.00001

class NeuralNetwork
{
public:
    NeuralNetwork();
    void setPicture(Picture* picture );
    void learn();
    void test();
    void forwardPropagation(int picture);
    void backPropagation(int epoch);

private:
    Neuron **network;
    Picture *pic;
    double **hiddenWeights;
    double **outputWeights;
    double *outputValues;
    bool flag;

};

#endif // NEURALNETWORK_H
