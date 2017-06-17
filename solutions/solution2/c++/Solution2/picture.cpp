#include "picture.h"
#define WIDTH 384
#define HEIGHT 286

Picture::Picture(QString idPicture, QString path, QString savePat)
{ 
    id=idPicture;
    imagePath=path+"/BioID_"+idPicture+".pgm";
    savePath=savePat+"/BioID_"+idPicture+".pgm";
    qDebug()<<"wszedlem";
}

QImage Picture::getImage()
{
    return image;
}

void Picture::read()
{
    qDebug()<<"wszedlem2";
    QImage picture(imagePath);
    startImage = picture;
}

void Picture::save()
{
    qDebug()<<"wszedlem3";
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
    QString path=openCVPath+"/bioid_"+id+".opencv";

    QFile OpenCV(path);
    OpenCV.open(QIODevice::ReadOnly);
    QTextStream stream(&OpenCV);

    int x = stream.readLine().toInt();
    int y = stream.readLine().toInt();
    int width = stream.readLine().toInt();

    image = image.copy(x, y, width, width*1.1);

}



void Picture::scale(int size)
{
    image = image.scaled(size, size);
}
