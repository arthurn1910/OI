#include "neuralnetwork.h"
#include <qDebug>

NeuralNetwork::NeuralNetwork()
{
    this->network=new Neuron*[3];
    this->network[0]=new Neuron[NETWORK_INPUT_LAYER];
    this->network[1]=new Neuron[NETWORK_HIDDEN_LAYER];
    this->network[2]=new Neuron[NETWORK_OUTPUT_LAYER];

    this->hiddenWeights=new double*[NETWORK_HIDDEN_LAYER];
    for(int i=0;i<NETWORK_HIDDEN_LAYER;i++){
        this->hiddenWeights[i]=new double[NETWORK_INPUT_LAYER];
    }

    this->outputWeights=new double*[NETWORK_OUTPUT_LAYER];
    for(int i=0;i<NETWORK_OUTPUT_LAYER;i++){
        this->hiddenWeights[i]=new double[NETWORK_HIDDEN_LAYER];
    }

    for (int i = 0; i < NETWORK_INPUT_LAYER; i++) {
        this->network[0][i] = Neuron(0, NETWORK_INPUT_LAYER, i,
                                     NETWORK_INPUT_LAYER,NETWORK_HIDDEN_LAYER,
                                     NETWORK_OUTPUT_LAYER);
    }

    for (int i = 0; i < NETWORK_HIDDEN_LAYER; i++) {
        this->network[1][i] = Neuron(1, NETWORK_INPUT_LAYER, i,
                                     NETWORK_INPUT_LAYER,NETWORK_HIDDEN_LAYER,
                                     NETWORK_OUTPUT_LAYER);
        this->network[1][i].setInNeurons(this->network[0]);
    }
    for (int i = 0; i < NETWORK_OUTPUT_LAYER; i++) {
        this->network[2][i] = Neuron(2, NETWORK_HIDDEN_LAYER, i,
                                         NETWORK_INPUT_LAYER,NETWORK_HIDDEN_LAYER,
                                         NETWORK_OUTPUT_LAYER);
        this->network[2][i].setInNeurons(this->network[1]);
    }

    for (int i = 0; i < NETWORK_HIDDEN_LAYER; i++) {
        this->network[1][i].setOutNeurons(this->network[2]);
    }
}

void NeuralNetwork::setPicture(Picture *picture){
    this->pic=picture;
}

void NeuralNetwork::learn(){
    flag=true;
    bool tmp56;
    for (int i = 0; i < 50; i++) {
        qDebug()<<QString::number(i);
        for(int picture=0;picture<913;picture++){
            this->pic[picture].save();
            if(picture==2)
                qDebug()<<"BladPicture "<<QString::number(picture);
            qDebug()<<"Picture "<<QString::number(picture);
            tmp56=this->pic[picture].getFlag();
            if(this->pic[picture].getFlag()==true){
                forwardPropagation(picture);
                backPropagation(i);
            }
        }
    }

}

void NeuralNetwork::forwardPropagation(int picture) {
    double* tmp = new double[NETWORK_INPUT_LAYER];
    double* tmp2 = new double[NETWORK_HIDDEN_LAYER];
    for (int k = 0; k < NETWORK_INPUT_LAYER; k++) {
        for(int y=0;y<this->pic[picture].getScale();y++){
            for(int x=0;x<this->pic[picture].getScale();x++){
                network[0][k].setInput(0, (this->pic[picture].getImage().pixel(x,y))/255);
                network[0][k].calculateOutput();
            }
        }
        tmp[k]=network[0][k].getOutput();
    }

    for (int k = 0; k < NETWORK_HIDDEN_LAYER; k++) {
        network[1][k].setInputs(tmp);
        network[1][k].calculateOutput();
        tmp2[k] = network[1][k].getOutput();
    }

    for (int k = 0; k < NETWORK_OUTPUT_LAYER; k++) {
        network[2][k].setInputs(tmp2);
        if(flag) {
            double tmpValue;
            if (k % 2 == 0){
                tmpValue=(this->pic[picture].getPositionLearn()[k] - this->pic[picture].getFaceX())/(this->pic[picture].getScaledX());
            }else{
                tmpValue=(this->pic[picture].getPositionLearn()[k] - this->pic[picture].getFaceY())/(this->pic[picture].getScaledY());
            }
            network[2][k].setDesiredValue(tmpValue);
        }
        network[2][k].calculateOutput();
    }
}

void NeuralNetwork::backPropagation(int epoch) {
    for (int k = 0; k < NETWORK_OUTPUT_LAYER; k++) {
        network[2][k].calculateError();
    }

    for (int k = 0; k < NETWORK_HIDDEN_LAYER; k++) {
        network[1][k].calculateError();
    }

    for (int k = 0; k < NETWORK_HIDDEN_LAYER; k++) {
        network[1][k].changeWeights();
        if (epoch == EPOCHS - 1) {
            hiddenWeights[k] = network[1][k].getWeights();
        }
    }

    for (int k = 0; k < NETWORK_OUTPUT_LAYER; k++) {
        network[2][k].changeWeights();
        if (epoch == EPOCHS - 1) {
            outputWeights[k] = network[2][k].getWeights();
        }
    }
}

void NeuralNetwork::test(){
    flag=false;

}
