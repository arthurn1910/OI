#include <iostream>
#include <cmath>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>

const double PRECISION = 0.15;
const int ELEMENT_OF_FACES_COUNT = 20;
const std::string ELEMENT_OF_FACES_ERROR[ELEMENT_OF_FACES_COUNT] {
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
    "TIP_OF_NOSE_ERROR",
    "RIGHT_NOSTRIL_ERROR",
    "LEFT_NOSTRIL_ERROR",
    "CENTRE_POINT_OF_UPPER_LIP_ERROR",
    "CENTRE_POINT_OF_LOWER_LIP_ERROR",
    "TIP_OF_CHIN_ERROR" };
	
	
const std::string ELEMENT_OF_FACES_SUCCESS[ELEMENT_OF_FACES_COUNT] {
    "RIGHT_EYE_PUPIL_SUCCESS",
    "LEFT_EYE_PUPIL_SUCCESS",
    "RIGHT_MOUTH_SUCCESS",
    "LEFT_MOUTH_SUCCESS",
    "RIGHT_OUTER_EYEBROW_SUCCESS",
    "RIGHT_INNER_EYEBROW_SUCCESS",
    "LEFT_INNER_EYEBROW_SUCCESS",
    "LEFT_OUTER_EYEBROW_SUCCESS",
    "RIGHT_TEMPLE_SUCCESS",
    "RIGHT_OUTER_EYE_SUCCESS",
    "RIGHT_INNER_EYE_SUCCESS",
    "LEFT_INNER_EYE_SUCCESS",
    "LEFT_OUTER_EYE_SUCCESS",
    "LEFT_TEMPLE_SUCCESS",
    "TIP_OF_NOSE_SUCCESS",
    "RIGHT_NOSTRIL_SUCCESS",
    "LEFT_NOSTRIL_SUCCESS",
    "CENTRE_POINT_OF_UPPER_LIP_SUCCESS",
    "CENTRE_POINT_OF_LOWER_LIP_SUCCESS",
    "TIP_OF_CHIN_SUCCESS" };

double **createTwoDimensionalTable(int firstDimension, int secondDimension);
double ***createTriDimensionalTable(int firstDimension, int secondDimension, int thirdDimension);
QTextStream *openFile(QString path, QString fileName);
void skipLines(QTextStream &stream, int n);
void readCoordinatesFromLine(QString line, double *saveIn);
double euclideanDistance(double *pointA, double *pointB);

int main(int argc, char *argv[])
{
    bool basic = false;
    QString testAnnotationsPath;
    QString resultAnnotationsPath;

    if (argc == 4) {
        if (std::atoi(argv[1]) == 0)
            basic = true;
        testAnnotationsPath = argv[2];
        resultAnnotationsPath = argv[3];
    } else {
       std::cout << "Invalid number of arguments!\n";
       return 1;
    }

    QDir testAnnotationsDir(testAnnotationsPath);
    QStringList namesOfFiles = testAnnotationsDir.entryList();
    namesOfFiles.erase(namesOfFiles.begin(), namesOfFiles.begin()+2);
    const int NUMBER_OF_FILES = namesOfFiles.size();

    double mean[ELEMENT_OF_FACES_COUNT] {0};
    double ***testPoints = createTriDimensionalTable(NUMBER_OF_FILES, ELEMENT_OF_FACES_COUNT, 2);
    double ***resultPoints = createTriDimensionalTable(NUMBER_OF_FILES, ELEMENT_OF_FACES_COUNT, 2);
    double **distances = createTwoDimensionalTable(NUMBER_OF_FILES, ELEMENT_OF_FACES_COUNT);
    QStringList namesOfInvalidFiles;
    double correctElementLocation[20] {0};


    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        QFile testAnnotationsFile(testAnnotationsPath + QDir::separator() + namesOfFiles.at(i));
        if(!testAnnotationsFile.open(QIODevice::ReadOnly)) {
            std::cout << "Cant open test file: " << i << ", " << (testAnnotationsPath+QDir::separator()+namesOfFiles.at(i)).toStdString() << "\n";
            return 2;
        }

        QFile resultAnnotationsFile(resultAnnotationsPath + QDir::separator() + namesOfFiles.at(i));
        if(!resultAnnotationsFile.open(QIODevice::ReadOnly)) {
            std::cout << "Cant open result file: " << i << ", " << (resultAnnotationsPath+QDir::separator()+namesOfFiles.at(i)).toStdString() << "\n";
            return 2;
        }

        QTextStream testAnnotationsStream(&testAnnotationsFile);
        QTextStream resultAnnotationsStream(&resultAnnotationsFile);

        // skip header
        skipLines(testAnnotationsStream, 3);
        skipLines(resultAnnotationsStream, 3);

        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            QString testLine = testAnnotationsStream.readLine();
            QString resultLine = resultAnnotationsStream.readLine();

            readCoordinatesFromLine(resultLine, resultPoints[i][j]);
            readCoordinatesFromLine(testLine, testPoints[i][j]);
        }

        double relativeDistance = euclideanDistance(testPoints[i][8], testPoints[i][13]);

        for (int j = 0; j < ELEMENT_OF_FACES_COUNT; j++) {
            if (basic && i >= 2)
                break;
            distances[i][j] = euclideanDistance(testPoints[i][j], resultPoints[i][j]) / relativeDistance;

            mean[j] += distances[i][j];

            if (distances[i][j] > PRECISION) {
                if (!namesOfInvalidFiles.contains(namesOfFiles.at(i)))
                    namesOfInvalidFiles.append(namesOfFiles.at(i));
            } else {
                correctElementLocation[j] += 1;
            }
        }

        testAnnotationsFile.close();
        resultAnnotationsFile.close();
    }

    // Compute mean
    for (int i = 0; i < ELEMENT_OF_FACES_COUNT; i++) {
        if (basic && i >= 2)
            break;
        mean[i] /= NUMBER_OF_FILES;
    }

    // First step of mean square error
    std::cout << "\nSquare mean error:\n";
    for (int i = 0; i < ELEMENT_OF_FACES_COUNT; i++) {
        if (basic && i >= 2)
            break;
        std::cout << "\t" << i << ". " << ELEMENT_OF_FACES_ERROR[i] << ":\t" << mean[i] << "\n";
    }

    std::cout << "\n\nCorrect location of all elements: " << (NUMBER_OF_FILES - namesOfInvalidFiles.size()) * 100.0 / (double) NUMBER_OF_FILES << " %\n\n";

    for (int i = 0; i < ELEMENT_OF_FACES_COUNT; i++) {
        if (basic && i >= 2)
            break;
        std::cout << "\t" << i << ". " << ELEMENT_OF_FACES_SUCCESS[i] << ":\t" << (correctElementLocation[i]) * 100.0 / NUMBER_OF_FILES << " %\n";
    }

    std::cout << "Correct files:\n";
    for (int j = 0; j < namesOfFiles.size(); j++) {
        if (!namesOfInvalidFiles.contains(namesOfFiles.at(j)))
            std::cout << "\t" << namesOfFiles.at(j).toStdString() << "\n";
    }

    if (namesOfInvalidFiles.size() > 0) {
        std::cout << "\nInvalid files:\n";
        for (int j = 0; j < namesOfInvalidFiles.size(); j++) {
            std::cout << "\t" << namesOfInvalidFiles.at(j).toStdString() << "\n";
        }
    }

    return 0;
}

double **createTwoDimensionalTable(int firstDimension, int secondDimension)
{
    double **newTable = new double*[firstDimension];
    for (int i = 0; i < firstDimension; i++) {
        newTable[i] = new double[secondDimension];
    }

    return newTable;
}

double ***createTriDimensionalTable(int firstDimension, int secondDimension, int thirdDimension)
{
    double ***newTable = new double**[firstDimension];
    for (int i = 0; i < firstDimension; i++) {
        newTable[i] = new double*[secondDimension];
        for (int j = 0; j < secondDimension; j++) {
            newTable[i][j] = new double[thirdDimension];
        }
    }

    return newTable;
}

void skipLines(QTextStream &stream, int n)
{
    for (int i = 0; i < n; i++)
    {
        stream.readLine();
    }
}


void readCoordinatesFromLine(QString line, double *saveIn)
{
    QStringList testCoordinatesString = line.split(" ");

    saveIn[0] = testCoordinatesString.at(0).toDouble();
    saveIn[1] = testCoordinatesString.at(1).toDouble();
}

double euclideanDistance(double *pointA, double *pointB)
{
    return std::sqrt(std::pow(pointA[0] - pointB[0], 2) +
                     std::pow(pointA[1] - pointB[1], 2));
}
