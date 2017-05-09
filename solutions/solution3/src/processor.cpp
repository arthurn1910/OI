#include <QImage>
#include <QDir>
#include <QDebug>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "processor.h"
#include <iostream>
#include <string>

QImage image;
int PARAM_TYPE = 0;
QString PARAM_DATABASE = "";
QString PARAM_ANNOTATIONS = "";

Processor::Processor()
{

}

int Processor::process(int type, QString database, QString results)
{
    if (not database.endsWith(QDir::separator()))
    {
        database.append(QDir::separator());
    }

    if (not results.endsWith(QDir::separator()))
    {
        results.append(QDir::separator());
    }

    PARAM_TYPE = type;
    PARAM_DATABASE = database;
    PARAM_ANNOTATIONS = results;

    return this->database();
}

int Processor::database()
{
    QDir filesDir(PARAM_DATABASE);
    QStringList files = filesDir.entryList();
    files.erase(files.begin(), files.begin() + 2);

    for (int file_id = 0; file_id < files.size(); file_id++)
    {
        QString f = files.at(file_id);
        f = f.replace("BioID_", "").replace(".pgm", "");
        QString imagePath = getImagePath(f);
        QString path = PARAM_DATABASE + imagePath;

        if (not image.load(path))
        {
            qDebug() << "Could not load image: " << path;

            image.detach();
            continue;
        }

        qDebug() << "Loaded image: " << path << " (" << image.width() << "x" << image.height() << ")";

        QImage &img = image;

        Bounds bounds = drawFaceRectangle();

        qDebug() << "Recognized face rectangle: x = [" << bounds.a << bounds.b << "], y = [" << bounds.c << bounds.d << "]";
        qDebug() << "Center of recognized face: [" << bounds.cx << bounds.cy << "]";

        int x, y;
        double colour;

        Face face;

        /*
         * Nose tip
         */
        colour = 0;
        face.noseTip = QPoint(0, 0);
        for (x = bounds.cx - 30; x < bounds.cx + 30; x++)
        {
            for (y = bounds.cy - 30; y < bounds.cy + 30; y++)
            {
                double c = calcColourOfPixelMask(x, y, 3, 3);

                if (c > colour)
                {
                    colour = c;
                    face.noseTip = QPoint(x + 2, y + 2);
                }
            }
        }

        qDebug() << "Nose tip: [" << face.noseTip.x() << face.noseTip.y() << "]";
        drawPoint(img, face.noseTip);

        /*
         * Nostrils
         */

        // left nostril
        colour = 999999;
        face.leftNostril = QPoint(0, 0);
        for (x = bounds.cx - 20; x < bounds.cx; x++)
        {
            for (y = bounds.cy; y < face.noseTip.y() + 40; y++)
            {
                double c = calcColourOfPixelMask(x, y, 3, 3);

                if (c < colour)
                {
                    colour = c;
                    face.leftNostril = QPoint(x + 1, y + 1);
                }
            }
        }

        qDebug() << "Left nostril: [" << face.leftNostril.x() << face.leftNostril.y() << "]";
        drawPoint(img, face.leftNostril);

        // right nostril
        colour = 999999;
        face.rightNostril = QPoint(0, 0);
        for (x = bounds.cx; x < bounds.cx + 20; x++)
        {
            for (y = bounds.cy; y < face.noseTip.y() + 40; y++)
            {
                double c = calcColourOfPixelMask(x, y, 3, 3);

                if (c < colour)
                {
                    colour = c;
                    face.rightNostril = QPoint(x + 1, y + 1);
                }
            }
        }

        qDebug() << "Right nostril: [" << face.rightNostril.x() << face.rightNostril.y() << "]";
        drawPoint(img, face.rightNostril);

        /*
         * Eye pupils
         */

        // left eye
        colour = 9999999;
        face.leftEyePupil = QPoint(0, 0);
        for (x = bounds.cx - 50; x < bounds.cx; x++)
        {
            for (y = bounds.cy - 50; y < face.noseTip.y(); y++)
            {
                double c = calcColourOfPixelMask(x, y, 3, 3);

                if (c < colour)
                {
                    colour = c;
                    face.leftEyePupil = QPoint(x + 1, y + 1);
                }
            }
        }

        qDebug() << "Left eye pupil: [" << face.leftEyePupil.x() << face.leftEyePupil.y() << "]";
        drawPoint(img, face.leftEyePupil);

        // right eye
        colour = 9999999;
        face.rightEyePupil = QPoint(0, 0);
        for (x = bounds.cx; x < bounds.cx + 50; x++)
        {
            for (y = bounds.cy - 50; y < face.noseTip.y(); y++)
            {
                double c = calcColourOfPixelMask(x, y, 3, 3);

                if (c < colour)
                {
                    colour = c;
                    face.rightEyePupil = QPoint(x + 1, y + 1);
                }
            }
        }

        qDebug() << "Right eye pupil: [" << face.rightEyePupil.x() << face.rightEyePupil.y() << "]";
        drawPoint(img, face.rightEyePupil);

        /*
         * Eye corners
         */
        int factor = (face.leftNostril.y() - face.noseTip.y()) * 1.75;

        // inner corners
        face.innerCornerOfLeftEye = QPoint(face.leftEyePupil.x() + factor, face.leftEyePupil.y());
        face.innerCornerOfRightEye = QPoint(face.rightEyePupil.x() - factor, face.rightEyePupil.y());

        qDebug() << "Inner corner of left eye: [" << face.innerCornerOfLeftEye.x() << face.innerCornerOfLeftEye.y() << "]";
        qDebug() << "Inner corner of right eye: [" << face.innerCornerOfRightEye.x() << face.innerCornerOfRightEye.y() << "]";
        drawPoint(img, face.innerCornerOfLeftEye);
        drawPoint(img, face.innerCornerOfRightEye);

        // outer corners
        face.outerCornerOfLeftEye = QPoint(face.leftEyePupil.x() - factor, face.leftEyePupil.y() + 3);
        face.outerCornerOfRightEye = QPoint(face.rightEyePupil.x() + factor, face.rightEyePupil.y() + 3);

        qDebug() << "Outer corner of left eye: [" << face.outerCornerOfLeftEye.x() << face.outerCornerOfLeftEye.y() << "]";
        qDebug() << "Outer corner of right eye: [" << face.outerCornerOfRightEye.x() << face.outerCornerOfRightEye.y() << "]";
        drawPoint(img, face.outerCornerOfLeftEye);
        drawPoint(img, face.outerCornerOfRightEye);

        /*
         * Eyebrows
         */

        // inner ends
        face.innerEndOfLeftEyebrow = QPoint(face.innerCornerOfLeftEye.x() + 2, face.innerCornerOfLeftEye.y() - factor * 0.9);
        face.innerEndOfRightEyebrow = QPoint(face.innerCornerOfRightEye.x() - 2, face.innerCornerOfRightEye.y() - factor * 0.9);

        qDebug() << "Inner end of left eyebrow: [" << face.innerEndOfLeftEyebrow.x() << face.innerEndOfLeftEyebrow.y() << "]";
        qDebug() << "Inner end of right eyebrow: [" << face.innerEndOfRightEyebrow.x() << face.innerEndOfRightEyebrow.y() << "]";
        drawPoint(img, face.innerEndOfLeftEyebrow);
        drawPoint(img, face.innerEndOfRightEyebrow);

        // outer ends
        face.outerEndOfLeftEyebrow = QPoint(face.outerCornerOfLeftEye.x() - (face.noseTip.x() - face.leftNostril.x()), face.innerEndOfLeftEyebrow.y());
        face.outerEndOfRightEyebrow = QPoint(face.outerCornerOfRightEye.x() + (face.rightNostril.x() - face.noseTip.x()), face.innerEndOfRightEyebrow.y());

        qDebug() << "Outer end of left eyebrow: [" << face.outerEndOfLeftEyebrow.x() << face.outerEndOfLeftEyebrow.y() << "]";
        qDebug() << "Outer end of right eyebrow: [" << face.outerEndOfRightEyebrow.x() << face.outerEndOfRightEyebrow.y() << "]";
        drawPoint(img, face.outerEndOfLeftEyebrow);
        drawPoint(img, face.outerEndOfRightEyebrow);

        /*
         * Temples
         */
        face.leftTemple = QPoint(face.outerEndOfLeftEyebrow.x() - 0.25 * (face.leftEyePupil.x() - face.outerCornerOfLeftEye.x()), face.outerEndOfLeftEyebrow.y() + ((face.outerCornerOfLeftEye.y() - face.outerEndOfLeftEyebrow.y()) / 2));
        face.rightTemple = QPoint(face.outerEndOfRightEyebrow.x() + 0.25 * (face.outerCornerOfRightEye.x() - face.rightEyePupil.x()), face.outerEndOfRightEyebrow.y() + ((face.outerCornerOfRightEye.y() - face.outerEndOfRightEyebrow.y()) / 2));

        qDebug() << "Left temple: [" << face.leftTemple.x() << face.leftTemple.y() << "]";
        qDebug() << "Left temple: [" << face.rightTemple.x() << face.rightTemple.y() << "]";
        drawPoint(img, face.leftTemple);
        drawPoint(img, face.rightTemple);

        /*
         * Mouth
         */

        // edges of lips
        face.outerEdgeUpperLip = QPoint(face.leftNostril.x() + ((face.rightNostril.x() - face.leftNostril.x()) / 2), face.leftNostril.y() + (face.noseTip.y() - face.outerCornerOfLeftEye.y()));
        face.outerEdgeLowerLip = QPoint(face.outerEdgeUpperLip.x(), face.outerEdgeUpperLip.y() + ((face.leftNostril.y() - face.innerCornerOfLeftEye.y()) / 2));

        qDebug() << "Center of upper outer edge of lip: [" << face.outerEdgeUpperLip.x() << face.outerEdgeUpperLip.y() << "]";
        qDebug() << "Center of lower outer edge of lip: [" << face.outerEdgeLowerLip.x() << face.outerEdgeLowerLip.y() << "]";
        drawPoint(img, face.outerEdgeUpperLip);
        drawPoint(img, face.outerEdgeLowerLip);

        // tip of chin
        face.tipOfChin = QPoint(face.outerEdgeLowerLip.x(), face.outerEdgeLowerLip.y() + (face.outerEdgeUpperLip.y() - face.leftNostril.y()));

        qDebug() << "Tip of chin: [" << face.tipOfChin.x() << face.tipOfChin.y() << "]";
        drawPoint(img, face.tipOfChin);

        // mouth corners
        face.leftMouthCorner = QPoint(face.outerEdgeUpperLip.x() - (face.outerEdgeUpperLip.y() - face.leftNostril.y()), face.outerEdgeUpperLip.y() + ((face.outerEdgeLowerLip.y() - face.outerEdgeUpperLip.y()) / 3));
        face.rightMouthCorner = QPoint(face.outerEdgeUpperLip.x() + (face.outerEdgeUpperLip.y() - face.leftNostril.y()), face.leftMouthCorner.y());

        qDebug() << "Left mouth corner: [" << face.leftMouthCorner.x() << face.leftMouthCorner.y() << "]";
        qDebug() << "Right mouth corner: [" << face.rightMouthCorner.x() << face.rightMouthCorner.y() << "]";
        drawPoint(img, face.leftMouthCorner);
        drawPoint(img, face.rightMouthCorner);

        image.detach();
        img.detach();

        saveAnnotation(f, face);
    }

    return 1;
}

int Processor::calcColourOfPixelMask(int startx, int starty, int rx, int ry)
{
    double colour = 0;

    for (int i = startx; i < startx + rx; i++)
    {
        for (int j = starty; j < starty + ry; j++)
        {
            colour += getPixelColour(i, j);
        }
    }

    return colour;
}

int Processor::getPixelColour(int x, int y)
{
    QRgb colour = image.pixel(x, y);

    return qGray(colour);
}

bool Processor::drawPoint(QImage &img, QPoint point)
{
    QPainter qPainter(&img);
    qPainter.setBrush(QBrush(Qt::white));
    qPainter.setPen(Qt::white);
    qPainter.drawEllipse(point.x() - 1, point.y() -1, 2, 2);
    qPainter.end();

    return true;
}

Bounds Processor::drawFaceRectangle()
{
    int offsetSides = 110, offsetUp = 75, offsetDown = 80;
    int width = image.width() - offsetSides * 2;
    int height = image.height() - offsetUp - offsetDown;

    Bounds bounds;
    bounds.a = offsetSides;
    bounds.b = bounds.a + width;
    bounds.c = offsetUp;
    bounds.d = bounds.c + height;
    bounds.cx = bounds.a + width / 2;
    bounds.cy = bounds.c + height / 2;

    return bounds;
}

QString Processor::getImagePath(QString string)
{
    return QString("BioID_%1.pgm").arg(string);
}

bool Processor::saveAnnotation(QString id, Face face)
{
    QString fileName = getImagePath(id);
    fileName = fileName.toLower().replace(".pgm", ".pts");

    QFile file(PARAM_ANNOTATIONS + fileName);
    QTextStream stream(&file);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't write to file";
        return 2;
    }

    stream << "version: 1" << endl;
    stream << "n_points: 20" << endl;
    stream << "{" << endl;

    stream << QString::number(face.rightEyePupil.x()) << " " << QString::number(face.rightEyePupil.y()) << endl;
    stream << QString::number(face.leftEyePupil.x()) << " " << QString::number(face.leftEyePupil.y()) << endl;

    if (PARAM_TYPE == 0)
    {
        for (int i = 2; i < 20; i++)
        {
            stream << QString::number(0) << " " << QString::number(0) << endl;
        }
    }
    else
    {
        stream << QString::number(face.rightMouthCorner.x()) << " " << QString::number(face.rightMouthCorner.y()) << endl;
        stream << QString::number(face.leftMouthCorner.x()) << " " << QString::number(face.leftMouthCorner.y()) << endl;
        stream << QString::number(face.outerEndOfRightEyebrow.x()) << " " << QString::number(face.outerEndOfRightEyebrow.y()) << endl;
        stream << QString::number(face.innerEndOfRightEyebrow.x()) << " " << QString::number(face.innerEndOfRightEyebrow.y()) << endl;
        stream << QString::number(face.innerEndOfLeftEyebrow.x()) << " " << QString::number(face.innerEndOfLeftEyebrow.y()) << endl;
        stream << QString::number(face.outerEndOfLeftEyebrow.x()) << " " << QString::number(face.outerEndOfLeftEyebrow.y()) << endl;
        stream << QString::number(face.rightTemple.x()) << " " << QString::number(face.rightTemple.y()) << endl;
        stream << QString::number(face.outerCornerOfRightEye.x()) << " " << QString::number(face.outerCornerOfRightEye.y()) << endl;
        stream << QString::number(face.innerCornerOfRightEye.x()) << " " << QString::number(face.innerCornerOfRightEye.y()) << endl;
        stream << QString::number(face.innerCornerOfLeftEye.x()) << " " << QString::number(face.innerCornerOfLeftEye.y()) << endl;
        stream << QString::number(face.outerCornerOfLeftEye.x()) << " " << QString::number(face.outerCornerOfLeftEye.y()) << endl;
        stream << QString::number(face.leftTemple.x()) << " " << QString::number(face.leftTemple.y()) << endl;
        stream << QString::number(face.noseTip.x()) << " " << QString::number(face.noseTip.y()) << endl;
        stream << QString::number(face.rightNostril.x()) << " " << QString::number(face.rightNostril.y()) << endl;
        stream << QString::number(face.leftNostril.x()) << " " << QString::number(face.leftNostril.y()) << endl;
        stream << QString::number(face.outerEdgeUpperLip.x()) << " " << QString::number(face.outerEdgeUpperLip.y()) << endl;
        stream << QString::number(face.outerEdgeLowerLip.x()) << " " << QString::number(face.outerEdgeLowerLip.y()) << endl;
        stream << QString::number(face.tipOfChin.x()) << " " << QString::number(face.tipOfChin.y()) << endl;
    }

    stream << "}" << endl;

    file.close();

    qDebug() << "Annotation saved";
    qDebug() << "";

    return true;
}
