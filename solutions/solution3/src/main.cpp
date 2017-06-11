#include <QCoreApplication>
#include <QtCore>

#include "neuralnetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int paramType;
    QString database, annotations;
    if (argc != 4)
    {
        qDebug() << "Invalid number of arguments!";
        return 1;
    }

    paramType = std::atoi(argv[1]);

    database = argv[2];
    if (!database.endsWith(QDir::separator()))
    {
        database.append(QDir::separator());
    }

    annotations = argv[3];
    if (!annotations.endsWith(QDir::separator()))
    {
        annotations.append(QDir::separator());
    }

    NeuralNetwork nn;
    nn.testNetwork(paramType, database, annotations);

    return 0;
}
