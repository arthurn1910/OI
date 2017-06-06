#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <QImage>
#include <cmath>
#include <iostream>
#include <string>
#include <clocale>
#include "pugixml/pugixml.hpp"
#include <QCoreApplication>
#include "neuralnetwork.h"

pugi::xml_document *loadNeuralNetworkSetup(bool basic);
double *readTrainImage(QString path);

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    std::setlocale(LC_NUMERIC,"en_US.UTF-8");

    const int ELEMENT_OF_FACES_COUNT = 20;
    if (argc != 4) {
        qDebug() << "Invalid number of arguments";
        return 1;
    }

    QString imagesPath = argv[2];
    if (!imagesPath.endsWith(QDir::separator())) {
        imagesPath.append(QDir::separator());
    }
    QString resultPath = argv[3];
    if (!resultPath.endsWith(QDir::separator())) {
        resultPath.append(QDir::separator());
    }
    QDir imagesDir(imagesPath);
    QStringList namesOfFiles = imagesDir.entryList();
    namesOfFiles.erase(namesOfFiles.begin(), namesOfFiles.begin() + 2);

    bool basic = false;
    if (std::atoi(argv[1]) == 0) {
        basic = true;
    }

    pugi::xml_document *doc = loadNeuralNetworkSetup(basic);
    if (doc == nullptr)
        return 3;

    NeuralNetwork *network = new NeuralNetwork(doc);

    for (int i = 0; i < namesOfFiles.size(); i++) {
        double *pixels = readTrainImage(imagesPath + namesOfFiles.at(i));

        double **coordinates = network->process(pixels);

        QString annotationFileName = namesOfFiles.at(i);
        annotationFileName.chop(3);
        annotationFileName = annotationFileName.toLower();
        annotationFileName.append("pts");

        QFile newFile(resultPath + annotationFileName);
        if (!newFile.open(QIODevice::WriteOnly)) {
            std::cout << "Couldn't write to: " << (resultPath + namesOfFiles.at(i)).toStdString();
            return 2;
        }

        QDir dir(QCoreApplication::applicationDirPath());

        dir.cdUp();
        dir.cd("data");
        dir.cd("test");

        QString openCvFile = dir.absolutePath() + QDir::separator();
        openCvFile.append(annotationFileName.left(annotationFileName.length() - 3));
        openCvFile.append("opencv");
        QFile fileCrop(openCvFile);
        fileCrop.open(QIODevice::ReadOnly);
        QTextStream fileStream(&fileCrop);
        int x = fileStream.readLine().toInt();
        int y = fileStream.readLine().toInt();
        fileCrop.close();

        QTextStream stream(&newFile);
        stream << "version: 1" << endl;
        stream << "n_points: 20" << endl;
        stream << "{" << endl;

        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            if (basic && j >= 2) {
                stream << QString::number(0) << " ";
                stream << QString::number(0) << endl;
                continue;
            }
            stream << QString::number(coordinates[j][0] + x) << " ";
            stream << QString::number(coordinates[j][1] + y) << endl;
        }

        stream << "}" << endl;

        newFile.close();

        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            delete [] coordinates[j];
        }
        delete coordinates;
    }

    return 0;
}


pugi::xml_document *loadNeuralNetworkSetup(bool basic)
{
    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("data");


    QString path = dir.absolutePath() + QDir::separator();

    if (basic) {
        path.append("basic.xml");
    } else {
        path.append("extended.xml");
    }

    pugi::xml_document *setup = new pugi::xml_document();
    pugi::xml_parse_result result = setup->load_file(path.toStdString().c_str());

    if (!result) {
        qDebug() << "XML parsed with errors.\n\n";
        return nullptr;
    }

    return setup;
}


double *readTrainImage(QString path)
{
    int lookUpTable[256];
    double contrastLevel = 1;
    for (int i = 0; i < 256; i++)
    {
        int pixelValue = contrastLevel * (i - 127) + 127;
        if (pixelValue < 0)
        {
            lookUpTable[i] = (uchar) 0;
        } else if (pixelValue > 255)
        {
            lookUpTable[i] = (uchar) 255;
        } else {
            lookUpTable[i] = (uchar) pixelValue;
        }
    }


    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("data");
    dir.cd("test");

    QString fileName = path.right(14);
    QString openCvFile = dir.absolutePath() + QDir::separator();
    openCvFile.append(fileName.left(fileName.length() - 3));
    openCvFile.append("opencv");
    QFile file(openCvFile);
    file.open(QIODevice::ReadOnly);
    QTextStream fileStream(&file);
    int x = fileStream.readLine().toInt();
    int y = fileStream.readLine().toInt();
    int width = fileStream.readLine().toInt();
    int height = fileStream.readLine().toInt() * 1.15;
    file.close();

    width += 4 - width % 4;
    height += 4 - height % 4;

    if (width == 4 && height == 4) {
        width = 384;
        height = 286;
        x = 0;
        y = 0;
    }

    QRect rect(x, y, width, height);
    const int IMAGE_SIZE = 32;

    double *outputImages = new double[IMAGE_SIZE * IMAGE_SIZE];
    QImage original(path);
    QImage cropped = original.copy(rect);

    QImage image = cropped.scaled(IMAGE_SIZE, IMAGE_SIZE);

    if (true) {
        uchar *photoData = cropped.bits();

        double *sobelTab = new double[width * height];
        double sobelX = 0;
        double sobelY = 0;
        uchar *processedData = new uchar[width * height];
        int maskHeight = 1;
        int maskWidth = 1;

        for (int i = maskHeight; i < height; i++) {
            for (int j = maskWidth; j < width; j++) {
                if (i + maskHeight >= height || j + maskWidth >= width) {
                    processedData[i * width + j] = photoData[i * width + j];
                    continue;
                }

                sobelX = photoData[(i-1) * width + (j+1)];// A2
                sobelX += 2* photoData[(i) * width + (j+1)];// A3
                sobelX += photoData[(i+1) * width + (j+1)];// A4
                sobelX -= photoData[(i-1) * width + (j-1)];// A0
                sobelX -= 2 * photoData[(i) * width + (j-1)];// A7
                sobelX -= photoData[(i+1) * width + (j-1)];// A6

                sobelY = photoData[(i-1) * width + (j-1)];// A0
                sobelY += 2* photoData[(i-1) * width + (j)];// A1
                sobelY += photoData[(i-1) * width + (j+1)];// A2
                sobelY -= photoData[(i+1) * width + (j-1)];// A6
                sobelY -= 2 * photoData[(i+1) * width + (j)];// A5
                sobelY -= photoData[(i+1) * width + (j+1)];// A4

                sobelTab[i * width + j] = std::sqrt(sobelX * sobelX + sobelY * sobelY);


                if (sobelTab[i * width + j] > 255) {
                    sobelTab[i * width + j] = 255;
                }
                processedData[i * width + j] = (uchar) sobelTab[i * width + j];
                processedData[i * width + j] = lookUpTable[processedData[i * width + j]];
            }
        }
        delete [] sobelTab;
        QImage sobolImage(processedData, cropped.width(), cropped.height(), cropped.bytesPerLine(), cropped.format());
        image = sobolImage.scaled(IMAGE_SIZE, IMAGE_SIZE);
    }

    outputImages = new double[IMAGE_SIZE * IMAGE_SIZE];
    uchar *bits = image.bits();

    int k = 0;
    double maxi = 0;
    double min = 255;
    for (int i = 0; i < IMAGE_SIZE; i += 1) {
        for (int j = 0; j < IMAGE_SIZE; j += 1) {
            outputImages[k++] = (double) bits[i * IMAGE_SIZE + j];
        }
    }


    for (int j = 0; j < k; j++) {
        if (outputImages[j] > maxi)
            maxi = outputImages[j];
        if (outputImages[j] < min)
            min = outputImages[j];
    }
    for (int j = 0; j < k; j++) {
        outputImages[j] = (1 / (maxi - min)) * outputImages[j];
    }

    return outputImages;
}
