#include "neuralnetwork.h"
#include "sigmoidfunction.h"
#include "forwardfunction.h"
#include "pugixml/pugixml.hpp"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <iostream>
#include <QImage>
#include <QDebug>
#include <string>

NeuralNetwork *createNetworkStrukture(QString structFileName);
double **readTrainImages(QDir imageDirectory);
double **readTrainAnnotations(QDir annotationDirectory);

int main(int argc, char *argv[])
{

    if (argc != 4)
        return 1;

    int iterations = std::atoi(argv[1]);
    double errorEpsilon = std::atof(argv[2]);
    QString pathToTrainDirectory = argv[3];
    if (!pathToTrainDirectory.endsWith(QDir::separator())) {
        pathToTrainDirectory.append(QDir::separator());
    }
    QDir imageDirectory(pathToTrainDirectory + "images");
    QDir annotationDirectory(pathToTrainDirectory + "annotations");

    int imagesCount = imageDirectory.entryList().size() - 2;

    NeuralNetwork *network =  createNetworkStrukture(pathToTrainDirectory + "networkStruct.txt");
    qDebug() << "Created neural network\n";

    double **input = readTrainImages(imageDirectory);
    double **desiredOutput = readTrainAnnotations(annotationDirectory);
    qDebug() << "Loaded data files\n";
    double *learingError = new double[iterations];

    for (int iter = 0; iter < iterations; iter++)
    {
        qDebug() << "Iteration: " << iter << "\n";
        double iterationError = 0;

        for (int currentImage = 0; currentImage < imagesCount; currentImage++) {
            //qDebug() << "\tImage: " << currentImage;
            double error = network->train(input[currentImage], desiredOutput[currentImage]);
            //qDebug() << ", error: " << error <<  "\n";
            iterationError += error;
        }

        learingError[iter] = iterationError / imagesCount;

        qDebug() << "\tMean Squared Error: " << iterationError / imagesCount << ".\n";
        if (iterationError < errorEpsilon) {
            break;
        }
    }
    double *out = network->process(input[0]);

    for (int i = 0; i < 40; i += 2) {
        qDebug() << out[i] << " " << out[i+1];
    }

    pugi::xml_document *doc = network->createXmlRepresentation();
    std::string str = pathToTrainDirectory.toStdString();
    str.append("extended.xml");
    doc->save_file(str.c_str());

    for (int i = 0; i < imagesCount; i++) {
        delete [] input[i];
        delete [] desiredOutput[i];
    }

    delete [] input;
    delete [] desiredOutput;
    delete [] learingError;
    delete network;

    return 0;
}


double **readTrainImages(QDir imageDirectory)
{
    QStringList imageFileNames = imageDirectory.entryList();
    imageFileNames.erase(imageFileNames.begin(), imageFileNames.begin()+2);

    double **outputImages = new double*[imageFileNames.size()];


    for (int currentImage = 0; currentImage < imageFileNames.size(); currentImage++) {
        QImage image(imageDirectory.absolutePath() + QDir::separator() + imageFileNames.at(currentImage));
        int offset = image.width() / 4;
        outputImages[currentImage] = new double[image.width() * image.height() / 2];
        uchar *bits = image.bits();

        int k = 0;
        double maxi = 0;
        double min = 255;
        for (int i = 0; i < image.height(); i += 3) {
            for (int j = 0; j < image.width() / 2; j += 3) {
                outputImages[currentImage][k++] = (double) bits[i * image.width() + j + offset];
            }
        }
        for (int j = 0; j < k; j++) {
            if (outputImages[currentImage][j] > maxi)
                maxi = outputImages[currentImage][j];
            if (outputImages[currentImage][j] < min)
                min = outputImages[currentImage][j];
        }
        for (int j = 0; j < k; j++) {
            outputImages[currentImage][j] = (1 / (maxi - min)) * outputImages[currentImage][j];
        }
    }

    return outputImages;
}

double **readTrainAnnotations(QDir annotationDirectory)
{
    QStringList annotationFileNames = annotationDirectory.entryList();
    annotationFileNames.erase(annotationFileNames.begin(), annotationFileNames.begin()+2);

    double **outputAnnotations = new double*[annotationFileNames.size()];

    for (int currentAnnotation = 0; currentAnnotation < annotationFileNames.size(); currentAnnotation++) {
        QFile file(annotationDirectory.absolutePath() + QDir::separator() + annotationFileNames.at(currentAnnotation));
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << " Cant open file!";
            return nullptr;
        }

        QTextStream in(&file);
        outputAnnotations[currentAnnotation] = new double[40];

        // Skipping heading
        in.readLine();
        in.readLine();
        in.readLine();

        for (int i = 0; i < 20; i++) {
            QString line = in.readLine();
            QStringList positions = line.split(QRegExp("\\s"));
            outputAnnotations[currentAnnotation][i * 2] = positions[0].toDouble();
            outputAnnotations[currentAnnotation][i * 2 + 1] = positions[1].toDouble();
        }

        file.close();
    }

    return outputAnnotations;
}

NeuralNetwork *createNetworkStrukture(QString structFileName)
{
    double biasParam = 0;
    double learningStep = 0;
    double momentumStep = 0;
    int *networkVector = nullptr;
    ActivationFunction **functions = nullptr;

    QFile file(structFileName);
    file.open(QIODevice::ReadOnly);
    QTextStream *textStream = new QTextStream(&file);

    QString line;
    QStringList args;

    line = textStream->readLine();

    args = line.split(",");
    biasParam = args.at(0).toDouble();
    qDebug() << "Bias: " << biasParam << "\n";
    learningStep = args.at(1).toDouble();
    qDebug() << "Learning step: " << learningStep << "\n";
    momentumStep = args.at(2).toDouble();
    qDebug() << "Momentum step: " << momentumStep << "\n";


    line = textStream->readLine();

    args = line.split(",");
    networkVector = new int[3];
    functions = new ActivationFunction*[2];

    qDebug() << "Layers: \n";
    for (int i = 0; i < 3; i++)
    {
        QString layerSize = args[i].mid(0, args.at(i).size() - 1);
        networkVector[i] = layerSize.toInt();
        qDebug() << "\t" << layerSize.toInt() << "\n";
        if (i > 0)
        {
            QString activ = args.at(i).mid(args.at(i).size() - 1, 1);
            if (activ == "s") {
                functions[i - 1] = new SigmoidFunction();
            } else if (activ == "l") {
                functions[i - 1] = new ForwardFunction();
            }
        }
    }
    file.close();

    NeuralNetwork *network = new NeuralNetwork(networkVector, biasParam, learningStep, momentumStep, functions);

    return network;
}
