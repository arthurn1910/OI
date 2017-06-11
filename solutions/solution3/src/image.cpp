#include "image.h"

Image::Image()
{

}

bool Image::load(QString path, QString id)
{
    QImage image(path);

    context = image;
    original = image;
    ident = id;;

    return true;
}

bool Image::save(QString path)
{
    if (context.save(path))
    {
        return true;
    }

    return false;
}

bool Image::getFace()
{
    QString path(QCoreApplication::applicationDirPath());
    path.append("/../data/annotations/bioid_");
    path.append(ident);
    path.append(".opencv");;

    QFile OpenCV(path);
    OpenCV.open(QIODevice::ReadOnly);
    QTextStream stream(&OpenCV);

    int x = stream.readLine().toInt();
    int y = stream.readLine().toInt();
    int width = stream.readLine().toInt() * 1.1;
    int height = width;

    QRect face(x, y, width, height);

    context = context.copy(face);

    return true;
}

bool Image::setBrightness(int factor)
{
    for (int i = 0; i < context.height(); ++i)
    {
        uchar *line = context.scanLine(i);

        for (int j = 0; j < context.width(); ++j)
        {
            int pixel = line[j];
            pixel += factor;

            line[j] = (pixel < 0) ? 0 : (pixel > 255) ? 255 : pixel;
        }
    }

    return true;
}

bool Image::setContrast(double factor)
{
    int pixels[256];
    for (int i = 0; i < 256; i++)
    {
        int pixel = factor * (i - 127) + 127;
        pixels[i] = (pixel) < 0 ? 0 : (pixel > 255) ? 255 : pixel;
    }

    for (int i = 0; i < context.height(); ++i)
    {
        uchar *line = context.scanLine(i);

        for (int j = 0; j < context.width(); ++j)
        {
            line[j] = pixels[line[j]];
        }
    }

    return true;
}

bool Image::applyFilter(int mask[3][3])
{
    const int maskSize = 3;

    int halfMask = maskSize / 2;
    for (int i = halfMask; i < context.height() - halfMask; ++i)
    {
        uchar *line = context.scanLine(i);

        for (int j = halfMask; j < context.width() - halfMask; ++j)
        {
            int pixel = 0;

            for (int k = i - halfMask, m = 0; k <= i + halfMask; ++k, ++m)
            {
                const uchar *maskLine = original.scanLine(k);

                for (int l = j - halfMask, n = 0; l <= j + halfMask; ++l, ++n)
                {
                    int value = maskLine[l] * mask[m][n];
                    pixel += value;
                }
            }

            line[j] = (pixel < 0) ? 0 : (pixel > 255) ? 255 : pixel;
        }
    }

    return true;
}

bool Image::resize(int dimension)
{
    context = context.scaled(dimension, dimension);

    return true;
}

QImage Image::getContext()
{
    return context;
}
