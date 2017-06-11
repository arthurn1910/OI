#include "neuralnetwork.h"

void NeuralNetwork::initNetwork()
{
    neuralNetwork << fc(NETWORK_SIZE, NETWORK_HIDDEN_LAYER)
                  << sigmoid()
                  << fc(NETWORK_HIDDEN_LAYER, 40);

    optimizer.alpha = NETWORK_ALPHA_START;
}

void NeuralNetwork::testNetwork(int paramType, QString database, QString annotations)
{
    initNetwork();

    QDir networkBinary(QCoreApplication::applicationDirPath());
    networkBinary.cdUp();
    networkBinary.cd("data");

    neuralNetwork.load(networkBinary.absoluteFilePath("neuralnetwork.bin").toStdString());

    std::vector<vec_t> testImages;

    QString directory(database);
    convertImages(directory, testImages);

    std::vector<vec_t> result = neuralNetwork.test(testImages);

    QDir dir(directory);
    QFileInfoList files = dir.entryInfoList();

    int i = 0;
    for (QFileInfo file : files)
    {
        if (file.isDir())
            continue;

        QString annotationFile = file.fileName().toLower().replace(".pgm", ".pts");
        QDir directory(annotations);

        QFile f(directory.absoluteFilePath(annotationFile));
        if (!f.open(QIODevice::WriteOnly))
        {
            qDebug() << "Can't open file for writing:" << directory.absoluteFilePath(annotationFile);
            continue;
        }

        directory = QCoreApplication::applicationDirPath();
        directory.cdUp();
        directory.cd("data");
        directory.cd("annotations");

        QFile openCV(directory.absoluteFilePath(annotationFile.replace(".pts", ".opencv")));
        openCV.open(QIODevice::ReadOnly);
        QTextStream fileStream(&openCV);
        int x = fileStream.readLine().toInt();
        int y = fileStream.readLine().toInt();
        int width = fileStream.readLine().toInt();
        int height = width;
        openCV.close();

        QTextStream stream(&f);
        stream << "version: 1" << endl;
        stream << "n_points: 20" << endl;
        stream << "{" << endl;

        for (int j = 0; j < 40; j += 2)
        {
            if (paramType == 0 && j >= 4)
            {
                stream << 0 << " ";
                stream << 0 << endl;
                continue;
            }

            float positionX = result[i][j];
            float positionY = result[i][j + 1];

            positionX += x;
            positionY += y;

            stream << positionX << " " << positionY << endl;
        }

        stream << "}" << endl;

        f.close();
        i++;
    }
}

bool NeuralNetwork::convertImage(const QString path, std::vector<vec_t>& data)
{
    vec_t d;

    QString ident;
    ident = path.right(8);
    ident = ident.left(4);

    Image image;
    image.load(path, ident);
    image.applyFilter(PrewittX);
    image.applyFilter(PrewittY);
    image.getFace();
    image.resize(DIMENSION);
    image.save("./face.pgm");

    QImage qImg(image.getContext());

    uchar *bits = qImg.bits();
    for (int i = 0; i < qImg.byteCount(); i++)
    {
        int pixel = bits[i];
        float value = pixel / 255.0;
        d.push_back(value);
    }

    data.push_back(d);

    return true;
}

bool NeuralNetwork::convertImages(const QString directory, std::vector<vec_t>& data)
{
    QDir dir(directory);
    QFileInfoList files = dir.entryInfoList();

    for (QFileInfo file : files)
    {
        if (file.isDir())
            continue;

        convertImage(file.absoluteFilePath(), data);
    }

    return true;
}

bool NeuralNetwork::loadAnnotations(const QString directory, std::vector<vec_t>& data)
{
    QDir dir(directory);
    QFileInfoList files = dir.entryInfoList();

    for (QFileInfo file : files)
    {
        if (file.isDir())
            continue;

        loadAnnotation(file.absoluteFilePath(), data);
    }

    return true;
}

bool NeuralNetwork::loadAnnotation(const QString path, std::vector<vec_t>& data)
{
    vec_t d;

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QDir directory(path);
    QString fileName = directory.absolutePath().right(14);
    fileName = fileName.replace(".pts", ".opencv");

    directory = QCoreApplication::applicationDirPath();
    directory.cdUp();
    directory.cd("data");
    directory.cd("annotations");

    QFile openCV(directory.absoluteFilePath(fileName));
    openCV.open(QIODevice::ReadOnly);
    QTextStream fileStream(&openCV);
    int x = fileStream.readLine().toInt();
    int y = fileStream.readLine().toInt();
    int width = fileStream.readLine().toInt();
    int height = width;
    openCV.close();

    QTextStream in(&file);

    in.readLine();
    in.readLine();
    in.readLine();

    for (int i = 0; i < 20; i++)
    {
        QString line = in.readLine();
        QStringList positions = line.split(QRegExp("\\s"));

        float positionX = positions[0].toFloat();
        float positionY = positions[1].toFloat();

        positionX -= x;
        positionY -= y;

        d.push_back(positionX);
        d.push_back(positionY);
    }

    data.push_back(d);

    return true;
}
