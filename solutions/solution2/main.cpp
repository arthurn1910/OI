#include <QCoreApplication>
#include <picture.h>
#include <qDebug>
#include <neuralnetwork.h>

#define SCALE 30

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString openPath = "C:/Users/A638852/Documents/sf/a";//argv[1];
    QString dataPath = "C:/Users/A638852/Documents/Solution2/data";  //argv[2];
    QString openCvPath= dataPath+"/OpenCV_addnotations";//
    QString saveResult="C:/Users/A638852/Documents/Solution2/data/result";//argv[3];
    QString trainPath= dataPath+"/points_20";
    QString id;
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
            picture[i]=new Picture(id,openPath,saveResult,openCvPath+"/train", trainPath);
            picture[i]->readPositionLearn();
        }else
            picture[i]=new Picture(id,openPath,saveResult,openCvPath+"/test");
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
