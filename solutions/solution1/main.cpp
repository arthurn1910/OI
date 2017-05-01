#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <QImage>
#include <cmath>
#include <iostream>
#include <string>
#include "pugixml/pugixml.hpp"

#include "neuralnetwork.h"

pugi::xml_document *loadNeuralNetworkSetup(bool basic);

int main(int argc, char *argv[])
{
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
    if (std::atoi(argv[1]) == 1) {
        basic = true;
    }

    pugi::xml_document *doc = loadNeuralNetworkSetup(basic);

    NeuralNetwork *network = new NeuralNetwork(doc);

    for (int i = 0; i < namesOfFiles.size(); i++) {
        QImage *image = new QImage(imagesPath + namesOfFiles.at(i));
        int pixelsInImage = image->width()*image->height();
        double *pixels = new double[pixelsInImage];
        uchar *bits = image->bits();
        for (int j = 0; j < pixelsInImage; j++) {
            pixels[j] = bits[j];
        }

        double **coordinates = network->process(pixels);

        QString annotationFileName = namesOfFiles.at(i);
        annotationFileName.chop(3);
        annotationFileName.append("pts");

        QFile newFile(resultPath + annotationFileName);
        if (!newFile.open(QIODevice::WriteOnly)) {
            std::cout << "Couldn't write to: " << (resultPath + namesOfFiles.at(i)).toStdString();
            return 2;
        }

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
            stream << QString::number(coordinates[j][0]) << " ";
            stream << QString::number(coordinates[j][1]) << endl;
        }

        stream << "}" << endl;

        newFile.close();
    }

    return 0;
}


pugi::xml_document *loadNeuralNetworkSetup(bool basic)
{
    QDir dir(QDir::current());
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
        std::cout << "XML parsed with errors.\n\n";
    }

    return setup;
}
