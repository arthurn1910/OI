#ifndef IMAGE_H
#define IMAGE_H

#include <QtCore>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QImage>

#define IMAGE_WIDTH 384
#define IMAGE_HEIGHT 286

class Image
{
    public:
        Image();
        bool load(QString path, QString ident);
        bool loadAnnotation();
        bool save(QString path);

        bool getFace();
        bool setBrightness(int factor);
        bool setContrast(double factor);
        bool applyFilter(int mask[3][3]);
        bool resize(int dimension);

        QImage getContext();

    private:
        QString ident;
        QImage context;
        QImage original;
};

#endif // IMAGE_H
