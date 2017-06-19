#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <picture.h>
#include <QDebug>
#include <neuron.h>

#define SIZE 30
#define NETWORK_INPUT_LAYER SIZE * SIZE
#define EPOCHS 1
#define NETWORK_HIDDEN_LAYER 200
#define NETWORK_OUTPUT_LAYER 40
#define ALPHA 0.00001

class NeuralNetwork
{
public:
    NeuralNetwork();
    void setPicture(Picture** picture );
    void learn();
    void test();
    void forwardPropagation(int picture,bool flag);
    void backPropagation(int epoch);
    void saveResult(QString path);
    void saveWeights();
    void readWeightsHidden();
    void readWeightsOutput();

private:
    Neuron **network;
    Picture **pic;
    double **hiddenWeights;
    double **outputWeights;
    double *outputValues;
    bool flag;

};

#endif // NEURALNETWORK_H
