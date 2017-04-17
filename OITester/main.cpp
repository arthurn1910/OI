#include <iostream>
#include <cmath>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    bool basic = true;
    QString testAnnotationsPath;
    QString resultAnnotationsPath;
    QString filesNames;
    int numberOfFiles;

    if (argc == 6) {
        if (argv[1] == "1")
            basic = false;
        testAnnotationsPath = argv[2];
        resultAnnotationsPath = argv[3];
        numberOfFiles = std::atoi(argv[4]);
        filesNames = argv[5];
    }
    QStringList namesOfFiles = filesNames.split(" ");
    const double PRECISION = 0.15;
    const int ELEMENT_OF_FACES_COUNT = 20;
    std::string elementsOfFaces[ELEMENT_OF_FACES_COUNT] {
        "RIGHT_EYE_PUPIL_ERROR",
        "LEFT_EYE_PUPIL_ERROR",
        "RIGHT_MOUTH_ERROR",
        "LEFT_MOUTH_ERROR",
        "RIGHT_OUTER_EYEBROW_ERROR",
        "RIGHT_INNER_EYEBROW_ERROR",
        "LEFT_INNER_EYEBROW_ERROR",
        "LEFT_OUTER_EYEBROW_ERROR",
        "RIGHT_TEMPLE_ERROR",
        "RIGHT_OUTER_EYE_ERROR",
        "RIGHT_INNER_EYE_ERROR",
        "LEFT_INNER_EYE_ERROR",
        "LEFT_OUTER_EYE_ERROR",
        "LEFT_TEMPLE_ERROR",
        "TIP_OF_NOSE",
        "RIGHT_NOSTRIL_ERROR",
        "LEFT_NOSTRIL_ERROR",
        "CENTRE_POINT_OF_UPPER_LIP_ERROR",
        "CENTRE_POINT_OF_LOWER_LIP_ERROR",
        "TIP_OF_CHIN_ERROR" };

    double mean[ELEMENT_OF_FACES_COUNT] {0};
    double squareError[ELEMENT_OF_FACES_COUNT] {0};
    double testPoints[numberOfFiles][ELEMENT_OF_FACES_COUNT][2];
    double resultPoints[numberOfFiles][ELEMENT_OF_FACES_COUNT][2];
    double distances[numberOfFiles][ELEMENT_OF_FACES_COUNT];
    bool correctLocation[numberOfFiles];

    for (int i = 0; i < numberOfFiles; i++) {
        QFile testAnnotationsFile(testAnnotationsPath+QDir::separator()+namesOfFiles.at(i));
        if(!testAnnotationsFile.open(QIODevice::ReadOnly)) {
        }

        QFile resultAnnotationsFile(resultAnnotationsPath+QDir::separator()+namesOfFiles.at(i));
        if(!resultAnnotationsFile.open(QIODevice::ReadOnly)) {
        }

        QTextStream testAnnotationsStream(&testAnnotationsFile);
        QTextStream resultAnnotationsStream(&resultAnnotationsFile);

        correctLocation[i] = true;
        // skip header
        testAnnotationsStream.readLine();
        testAnnotationsStream.readLine();
        testAnnotationsStream.readLine();
        resultAnnotationsStream.readLine();
        resultAnnotationsStream.readLine();
        resultAnnotationsStream.readLine();
        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            QString testLine = testAnnotationsStream.readLine();
            QStringList testCoordinatesString = testLine.split(" ");
            QString resultLine = resultAnnotationsStream.readLine();
            QStringList resultCoordinatesString = resultLine.split(" ");

            resultPoints[i][j][0] = resultCoordinatesString.at(0).toDouble();
            resultPoints[i][j][1] = resultCoordinatesString.at(1).toDouble();

            testPoints[i][j][0] = testCoordinatesString.at(0).toDouble();
            testPoints[i][j][1] = testCoordinatesString.at(1).toDouble();
        }
        double relativeDistance = std::sqrt(std::pow(testPoints[i][19][0] - testPoints[i][14][0], 2) +
                                            std::pow(testPoints[i][19][1] - testPoints[i][14][1], 2));
        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            distances[i][j] = std::sqrt(std::pow(testPoints[i][j][0] - resultPoints[i][j][0], 2) +
                                        std::pow(testPoints[i][j][1] - resultPoints[i][j][1], 2)) / relativeDistance;
            mean[j] += distances[i][j];
            if (distances[i][j] > PRECISION) {
                correctLocation[i] = false;
            }
        }

        testAnnotationsFile.close();
        resultAnnotationsFile.close();
    }

    for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
        mean[j] /= numberOfFiles;
    }
    for (int i = 0; i < numberOfFiles; i++) {
        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            squareError[j] += std::pow(distances[i][j] - mean[j], 2);
        }
    }
    for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
        squareError[j] /= numberOfFiles;
    }

    std::cout << "Square mean error:\n";
    for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
        std::cout << "\t" << j << ". " << elementsOfFaces[j] << ": " << squareError[j] << "\n";
    }

    std::cout << "\nInvalid files:\n";
    for (int j = 0; j < numberOfFiles; j++) {
        if (!correctLocation[j]) {
            std::cout << namesOfFiles.at(j).toStdString() << "\n";
        }
    }

    return 0;
}
