#include "neuralnetwork.h"
#include <qDebug>
#include <QTextStream>

NeuralNetwork::NeuralNetwork()
{
    this->network=new Neuron*[3];
    this->outputValues=new double[40];
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

void NeuralNetwork::setPicture(Picture **picture){
    this->pic=picture;
}

void NeuralNetwork::learn(){
    flag=true;
    for (int i = 0; i < EPOCHS; i++) {
        qDebug()<<"EPOCHS"<<QString::number(i);
        for(int picture=0;picture<913;picture++){
            qDebug()<<"PICTURE "<<QString::number(picture);
            if(this->pic[picture]->getFlag()==true){
                forwardPropagation(picture,flag);
                backPropagation(i);
            }
        }
    }

    //saveWeights();

}

void NeuralNetwork::forwardPropagation(int picture,bool flag) {
    double* tmp = new double[NETWORK_INPUT_LAYER];
    double* tmp2 = new double[NETWORK_HIDDEN_LAYER];
    for (int k = 0; k < NETWORK_INPUT_LAYER; k++) {
        for(int y=0;y<this->pic[picture]->getScale();y++){
            for(int x=0;x<this->pic[picture]->getScale();x++){
                network[0][k].setInput(0, (this->pic[picture]->getImage().pixel(x,y))/255);
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
                tmpValue=(this->pic[picture]->getPositionLearn()[k] - this->pic[picture]->getFaceX())/(this->pic[picture]->getScaledX());
            }else{
                tmpValue=(this->pic[picture]->getPositionLearn()[k] - this->pic[picture]->getFaceY())/(this->pic[picture]->getScaledY());
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
    //readWeightsHidden();
    //readWeightsOutput();
    flag=false;
    for(int picture=913;picture<=1520;picture++){
        qDebug()<<"Picture "+QString::number(picture);
        if(picture==1082)
            qDebug()<<"Picture "+QString::number(picture);
        if(this->pic[picture]->getFaceWidth()>10){
            forwardPropagation(picture,flag);
            for(int j = 0;j<NETWORK_OUTPUT_LAYER;j++) {
                if(j%2==0){
                    double yuy=network[2][j].getOutput()*pic[picture]->getScaledX()+pic[picture]->getFaceX();
                    outputValues[j] = yuy;
                }else{
                    outputValues[j] = network[2][j].getOutput()*pic[picture]->getScaledX()+pic[picture]->getFaceY();
                }
            }
        }else{
            for(int j = 0;j<NETWORK_OUTPUT_LAYER;j++) {
                    outputValues[j] = 0;
            }
        }
        qDebug()<<"Save "+QString::number(picture);
        saveResult(pic[picture]->getSaveResultPath());

    }

}

void NeuralNetwork::saveResult(QString path){
    QFile save(path);
    save.open(QIODevice::WriteOnly);
    QTextStream stream(&save);
    stream << "version: 1" << endl;
    stream << "n_points: 20" << endl;
    stream << "{" << endl;
    for (int j = 0; j < 40; j += 2)
    {
        stream << outputValues[j] << " " << outputValues[j+1] << endl;
    }

    stream << "}" << endl;

    save.close();
}

void NeuralNetwork::saveWeights(){
    QFile save("C:/Users/A638852/Documents/Solution2/data/weightsHidden.txt");
    save.open(QIODevice::WriteOnly);
    QTextStream stream(&save);
    for(int i=0;i<NETWORK_HIDDEN_LAYER;i++){
        for(int j=0;j<NETWORK_INPUT_LAYER;j++)
            stream << this->hiddenWeights[i][j] <<endl;
    }
    save.close();
    QFile saveO("C:/Users/A638852/Documents/Solution2/data/weightsOutput.txt");
    saveO.open(QIODevice::WriteOnly);
    QTextStream streamO(&saveO);
    for(int i=0;i<NETWORK_OUTPUT_LAYER;i++){
        for(int j=0;j<NETWORK_HIDDEN_LAYER;j++)
            streamO << this->outputWeights[i][j] <<endl;
    }
    saveO.close();
}

void NeuralNetwork::readWeightsHidden(){
    QFile readHiddenWeights("C:/Users/A638852/Documents/Solution2/data/weightsHidden.txt");
    readHiddenWeights.open(QIODevice::ReadOnly);
    QTextStream streamReadH(&readHiddenWeights);
    QString tmp="0";
    double weight;
    for(int i=0;i<NETWORK_HIDDEN_LAYER;i++){
        for(int j=0;j<NETWORK_INPUT_LAYER;j++){
            tmp=streamReadH.readLine();
            qDebug()<<QString::number(i)+" "+QString::number(j)+" -"+tmp;
            weight=tmp.toDouble();
            this->hiddenWeights[i][j] = weight;
        }
    }
    readHiddenWeights.close();
//    QFile readOutputWeights("C:/Users/A638852/Documents/Solution2/data/weightsOutput.txt");
//    readOutputWeights.open(QIODevice::ReadOnly);
//    QTextStream streamReadO(&readOutputWeights);
//    QString tmp2;
//    double weight2;
//    for(int p=0;p<NETWORK_OUTPUT_LAYER;p++){
//        qDebug()<<"p "<<QString::number(p);
//        for(int o=0;o<NETWORK_HIDDEN_LAYER;o++){
//            qDebug()<<"o "<<QString::number(o);
//            tmp2=streamReadO.readLine();
//            weight2=tmp2.toDouble();
//            this->outputWeights[p][o] = weight2;
//        }
//    }
//    readOutputWeights.close();
//    qDebug()<<"OUTPUT";
//    for(int i=0;i<NETWORK_OUTPUT_LAYER;i++){
//        for(int j=0;j<NETWORK_HIDDEN_LAYER;j++)
//            qDebug()<<QString::number(this->outputWeights[i][j]);
//    }
    qDebug()<<"HIDDEN";
    for(int i=0;i<NETWORK_HIDDEN_LAYER;i++){
        for(int j=0;j<NETWORK_INPUT_LAYER;j++)
            qDebug()<<"i "<<QString::number(i)<<" j "<<QString::number(j)<<"   "<<
                      QString::number(hiddenWeights[i][j]);
    }

    qDebug()<<"END0";
}

void NeuralNetwork::readWeightsOutput(){
    QFile readOutputWeights("C:/Users/A638852/Documents/Solution2/data/weightsOutput.txt");
    readOutputWeights.open(QIODevice::ReadOnly);
    QTextStream streamReadO(&readOutputWeights);
    QString tmp2;
    double weight2;
    for(int p=0;p<NETWORK_OUTPUT_LAYER;p++){
        qDebug()<<"p "<<QString::number(p);
        for(int o=0;o<NETWORK_HIDDEN_LAYER;o++){
            qDebug()<<"o "<<QString::number(o);
            tmp2=streamReadO.readLine();
            weight2=tmp2.toDouble();
            this->outputWeights[p][o] = weight2;
        }
    }
//    readOutputWeights.close();
//    qDebug()<<"OUTPUT";
//    for(int i=0;i<NETWORK_OUTPUT_LAYER;i++){
//        for(int j=0;j<NETWORK_HIDDEN_LAYER;j++)
//            qDebug()<<QString::number(this->outputWeights[i][j]);
//    }
    qDebug()<<"END0";
}
