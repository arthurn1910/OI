#ifndef PICTURE_H
#define PICTURE_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QImage>

class Picture
{
public:
    Picture(QString idPicture, QString path, QString savePath, QString openCVPath);
    Picture(QString idPicture, QString path, QString savePath, QString openCVPath, QString trainPath);
    void read();
    void loadAnnotation();
    void save();
    QImage getImage();
    void getFacePosition();
    void scalePicture(int size);
    QString getSavePath();
    QString getId();
    QString getTrainPath();
    void readPositionLearn();
    int getFaceWidth();
    int getFaceHeight();
    int getScale();
    double *getPositionLearn();
    int getFaceX();
    int getFaceY();
    double getScaledX();
    double getScaledY();
    bool getFlag();
private:
    QImage image;
    QImage startImage;
    QString id;
    QString imagePath;
    QString savePath;
    QString openCVPath;
    QString trainPath;
    int faceX;
    int faceY;
    int faceWidth;
    int faceHeight;
    double positionLearn[40];
    int scale;
    double scaledX;
    double scaledY;
    bool flag;
};

#endif // PICTURE_H
