#include <QCoreApplication>
#include <picture.h>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <QImage>
#include <cmath>
#include <iostream>
#include <string>
#include <clocale>
#include <QCoreApplication>
#include <neuralnetwork.h>
#define SCALE 30

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc != 4) {
        qDebug() << "Invalid number of arguments";
        return 1;
    }

    QString openPath = argv[2];
    if (!openPath.endsWith(QDir::separator())) {
        openPath.append(QDir::separator());
    }
    QString openPathTest=openPath;
    QString openPathLearn=openPath.replace(QString("test/images"), QString("train/images"));
    qDebug() <<openPathLearn<<" "<<openPathTest;
    QString saveResult= argv[3];
    if (!saveResult.endsWith(QDir::separator())) {
        saveResult.append(QDir::separator());
    }
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("data");
    QString dataPath = dir.absolutePath() + QDir::separator(); 
    QString openCvPath= dataPath+"OpenCV_addnotations";
    QString trainPath= dataPath+"/points_20";
    QString id;
    qDebug() <<openCvPath;
    Picture **picture=new Picture*[1521];
    for(int i=0;i<=1520;i++){
        qDebug()<<QString::number(i);
        if(i<10){
            id="000"+QString::number(i);
        }else if(i<100){
            id="00"+QString::number(i);
        }else if(i<913){
            id="0"+QString::number(i);
        }else if(i<1000){
            id="0"+QString::number(i);
        }else{
            id=QString::number(i);
        }
        if(i<913){
            picture[i]=new Picture(id,openPathLearn,saveResult,openCvPath+"/train", trainPath);
            picture[i]->readPositionLearn();
        }else
            picture[i]=new Picture(id,openPathTest,saveResult,openCvPath+"/test");
        picture[i]->read();
        picture[i]->getFacePosition();
        if(picture[i]->getFlag()==true){
            picture[i]->scalePicture(SCALE);
        }
    }

    NeuralNetwork* neuralNetwork=new NeuralNetwork();
    neuralNetwork->setPicture(picture);
    neuralNetwork->learn();
    neuralNetwork->test();
    return 0;
}
