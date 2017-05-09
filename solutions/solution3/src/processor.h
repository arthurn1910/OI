#ifndef PROCESSING_H
#define PROCESSING_H

#include <QPoint>

struct Bounds {
    int a, b, c, d;
    int cx, cy;
};

struct Face {
    QPoint noseTip,
    leftNostril,
    rightNostril,
    rightEyePupil,
    leftEyePupil,
    innerCornerOfLeftEye,
    outerCornerOfLeftEye,
    innerCornerOfRightEye,
    outerCornerOfRightEye,
    innerEndOfLeftEyebrow,
    outerEndOfLeftEyebrow,
    innerEndOfRightEyebrow,
    outerEndOfRightEyebrow,
    leftTemple,
    rightTemple,
    tipOfChin,
    outerEdgeUpperLip,
    outerEdgeLowerLip,
    leftMouthCorner,
    rightMouthCorner;
};

class Processor
{
public:
    Processor();
    int process(int type, QString database, QString results);
private:
    int database();
    QString getImagePath(int i);
    int calcColourOfPixelMask(int startx, int starty, int rx, int ry);
    int getPixelColour(int x, int y);
    bool drawPoint(QImage &img, QPoint coords);
    Bounds drawFaceRectangle();
    bool saveAnnotation(int id, Face face);
};

#endif // PROCESSING_H
