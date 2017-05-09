#include "neuralnetwork.h"
#include <cmath>
#include <QDebug>

NeuralNetwork::NeuralNetwork(int *networkVector, double bias, double learningStep, double momentumStep, ActivationFunction **activation) {
    // schemat sieci - liczba neutronów w warstwie
    this->networkVector = networkVector;
    // strukutura sieci
    this->neurons = new Neuron**[2];
    // tabela wyjść z warstw
    this->neuronsLastOutputs = new double*[3];

    // tworzymy tabele na podstawie schematu sieci
    for (int i = 0; i < 3; i++)
    {
        neuronsLastOutputs[i] = new double[networkVector[i]];
    }
    // tworzymy strukure na podstawie schematu bez werstwy wejściowej
    for (int i = 0; i < 2; i++)
    {
        neurons[i] = new Neuron*[networkVector[i + 1]];
    }
    // pętla przechodzi przez warstwy sieci
    for (int i = 0; i < 2; i++)
    {
        // pętla przechodzi przez perceptrony w warstwie
        for (int j = 0; j < networkVector[i + 1]; j++)
        {
            //qDebug() << "Warstwa: " << i << ", neuron:" << j << endl;
            // tworzymy perceptrony i przypisujemy funkcje aktywacji
            neurons[i][j] = new Neuron(networkVector[i], bias, learningStep, momentumStep);
            neurons[i][j]->activation = activation[i];
        }
    }
}

double *NeuralNetwork::process(double *input)
{
    // symulujemy wyjście z warstwy wejściowej
    neuronsLastOutputs[0] = input;

    // pętla przechodząca przez warstwy sieci
    for (int i = 0; i < 2; i++)
    {
        // pętla przechodząca przez perceptrony w warstwie
        for (int j = 0; j < networkVector[i + 1]; j++)
        {
            // obliczmy wyjścia perceptronów
            neuronsLastOutputs[i + 1][j] = neurons[i][j]->feedForward(neuronsLastOutputs[i]);
        }
    }
    // zwracamy wyjścia perceptronów z ostatniej warstwy
    //return Arrays.copyOf(neuronsLastOutputs[neuronsLastOutputs.length - 1], neuronsLastOutputs[neuronsLastOutputs.length - 1].length);
    return neuronsLastOutputs[2];
}

double NeuralNetwork::train(double *input, double *desiredOutput)
{
    double squaredError = 0;

    double *output = process(input);

    // obliczamy błąd średnio kwadratowy
    for (int i = 0; i < networkVector[2]; i++)
    {
        qDebug() << "D: " << desiredOutput[i] << ", O: " << output[i] << ", E: " << squaredError << "\n";
        squaredError += std::pow(desiredOutput[i] - output[i], 2);
    }
    // pętla przechodząca przez perceptrony w ostatniej wartswie
    for (int i = 0; i < networkVector[2]; i++)
    {
        // obliczamy delte w każdym perceptronie
        neurons[1][i]->deltaUpdate(desiredOutput[i]);
//        qDebug() << "Neuron: 1::" << i << "\n";
//        qDebug() << "\tDelta: " << neurons[1][i]->delta << "\n";
    }

    // pętla przechodząca przez perceptrony w warstwie
    for (int j = 0; j < networkVector[1]; j++)
    {
        // tworzymy tabele na delta * waga dla każdego perceptronu
        double *upperLayerDeltasTimesWeights = new double[networkVector[2]];
        // pętla przechodząca przez perceptrony warstwy wyższej
        for (int k = 0; k < networkVector[2]; k++)
        {
            // obliczamy iloczyny
            upperLayerDeltasTimesWeights[k] = neurons[1][k]->delta * neurons[1][k]->weights[j];
        }
        // aktualizaujemy delte w perceptronie
        neurons[0][j]->deltaUpdate(upperLayerDeltasTimesWeights, networkVector[2]);
//        qDebug() << "Neuron: 0::" << j << "\n";
//        qDebug() << "\tDelta: " << neurons[0][j]->delta << "\n";

        delete [] upperLayerDeltasTimesWeights;
    }

    // pętla przechodząca przez warstwy sieci bez warstwy wejściowej
    for (int i = 0; i < 2; i++)
    {
        // pętla przechodząca przez perceptrony w warstwie
        for (int j = 0; j < networkVector[i + 1]; j++)
        {
            neurons[i][j]->weightsUpdate(neuronsLastOutputs[i]);
        }
    }

    return squaredError;
}

double ***NeuralNetwork::getNetworkState()
{
    double ***networkState = new double**[3];
    for (int i = 0; i < 3; i++)
    {
        networkState[i] = new double*[networkVector[i]];
    }

    double *inputLayer = new double[networkVector[0] + 1];
    for (int i = 0; i < networkVector[0] + 1; i++)
    {
        if (i == networkVector[0]) {
            inputLayer[i] = 0;
        } else {
            inputLayer[i] = 1;
        }
    }
    // pętla przechodząca przez wartsty
    for (int i = 0; i < 3; i++)
    {
        // pętla przechodząca przez perceptrony w warstwie
        for (int j = 0; j < networkVector[i]; j++)
        {
            if (i == 0)
            {
                networkState[i][j] = inputLayer;
            }
            else
            {
                networkState[i][j] = neurons[i - 1][j]->getWeights();
            }
        }
    }

    return networkState;
}

pugi::xml_document *NeuralNetwork::createXmlRepresentation()
{
    pugi::xml_document *doc = new pugi::xml_document();


    pugi::xml_node neuralNetworkNode = doc->append_child("NeuralNetwork");
    neuralNetworkNode.append_attribute("layerCount") = 2;

    pugi::xml_node firstLayerNode = neuralNetworkNode.append_child("NeuralNetworkLayer");
    firstLayerNode.append_attribute("neuronCount") = networkVector[1];
    for (int i = 0; i < networkVector[1]; i++) {
        pugi::xml_node neuronNode = firstLayerNode.append_child("Neuron");
        neurons[0][i]->fillNeuronNode(neuronNode);
    }

    pugi::xml_node secondLayerNode = neuralNetworkNode.append_child("NeuralNetworkLayer");
    secondLayerNode.append_attribute("neuronCount") = networkVector[2];
    for (int i = 0; i < networkVector[2]; i++) {
        pugi::xml_node neuronNode = secondLayerNode.append_child("Neuron");
        neurons[1][i]->fillNeuronNode(neuronNode);
    }

    return doc;
}
