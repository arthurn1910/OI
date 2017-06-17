#ifndef PICTURE_H
#define PICTURE_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QImage>

class Picture
{
public:
    Picture(QString idPicture, QString path, QString savePath);
    void read();
    void loadAnnotation();
    void save();
    QImage getImage();
    void getFacePosition();
    void scale(int size);
    QString getSavePath();
    QString getId();
    QString getTrainPath();

private:
    QImage image;
    QImage startImage;
    QString id;
    QString imagePath;
    QString savePath;
    QString openCVPath;
    QString trainPath;
};

#endif // PICTURE_H
