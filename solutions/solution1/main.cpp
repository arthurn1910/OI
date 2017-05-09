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
    //QDir dir("E:\\Virt\\shere\\final\\solution1\\bin");
    //QDir dir("/media/sf_shere/final/solution1/bin");

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
    QImage image(path);
    int offset = image.width() / 4;
    double *outputImages = new double[6144];
    uchar *bits = image.bits();

    int k = 0;
    double maxi = 0;
    double min = 255;
    for (int i = 0; i < image.height(); i += 3) {
        for (int j = 0; j < image.width() / 2; j += 3) {
            outputImages[k++] = (double) bits[i * image.width() + j + offset];
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
