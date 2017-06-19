#include "picture.h"
#include <stdlib.h>
#define WIDTH 384
#define HEIGHT 286

Picture::Picture(QString idPicture, QString path, QString saveResult, QString openCv)
{ 
    id=idPicture;
    imagePath=path+"BioID_"+idPicture+".pgm";
    openCVPath=openCv+"/bioid_"+id+".opencv";
    saveResultPath=saveResult+"/bioid_"+id+".pts";
}

Picture::Picture(QString idPicture, QString path, QString savePath, QString openCv, QString trainPath)
{
    id=idPicture;
    imagePath=path+"BioID_"+idPicture+".pgm";
    this->savePath=savePath+"/BioID_"+idPicture+".pgm";
    openCVPath=openCv+"/bioid_"+id+".opencv";
    this->trainPath=trainPath+"/bioid_"+idPicture+".pts";
}

void Picture::readPositionLearn(){
    QFile train(trainPath);
    train.open(QIODevice::ReadOnly);
    QTextStream stream(&train);
    stream.readLine();
    stream.readLine();
    stream.readLine();
    QString tmp;
    for(int i=0;i<40;i+=2){
        QString line = stream.readLine();
        QStringList positions = line.split(QRegExp(" "));
        tmp=positions.at(0);
        this->positionLearn[i] = tmp.toDouble();//stream.readLine().toDouble();
        tmp=positions.at(1);
        this->positionLearn[i+1] = tmp.toDouble();// stream.readLine().toDouble();
    }
}

void Picture::read()
{
    QImage picture(imagePath);
    startImage = picture;
}

void Picture::save()
{
    image.save(savePath);
}

QString Picture::getSavePath(){
    return savePath;
}
QString Picture::getTrainPath(){
    return trainPath;
}

QString Picture::getId(){
    return id;
}



void Picture::getFacePosition()
{
    QFile OpenCV(openCVPath);
    OpenCV.open(QIODevice::ReadOnly);
    QTextStream stream(&OpenCV);

    this->faceX = stream.readLine().toInt();
    this->faceY = stream.readLine().toInt();
    this->faceWidth = stream.readLine().toInt();
    this->faceHeight=this->faceWidth*1.1;

    if(this->faceWidth<25){
        flag=false;
        qDebug()<<"blad";
    }else{
        flag=true;
        image = startImage.copy(this->faceX,this->faceY,this->faceWidth,this->faceHeight);
    }

}

int Picture::getFaceHeight(){
    return this->faceHeight;
}

int Picture::getFaceWidth(){
    return this->faceWidth;
}

QImage Picture::getImage(){
    return this->image;
}

int Picture::getScale(){
    return this->scale;
}

void Picture::scalePicture(int size)
{
    image = image.scaled(size, size);
    this->scale=size;
    this->scaledX=this->getFaceWidth()/((double) size);
    this->scaledY=this->getFaceHeight()/((double) size);
}

double* Picture::getPositionLearn(){
    return this->positionLearn;
}

int Picture::getFaceX(){
    return this->faceX;
}

int Picture::getFaceY(){
    return this->faceY;
}


double Picture::getScaledX(){
    return this->scaledX;
}

double Picture::getScaledY(){
    return this->scaledY;
}

bool Picture::getFlag(){
    return this->flag;
}

QString Picture::getSaveResultPath(){
    return this->saveResultPath;
}

