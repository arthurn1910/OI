#ifndef CUSTOMTABFORIMAGE_H
#define CUSTOMTABFORIMAGE_H

#include <QWidget>
#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QCheckBox>
#include <vector>

namespace Ui {
class CustomTabForImage;
struct Annotation;
}

struct Annotation{
   int x;
   int y;
   QString label;
};

class CustomTabForImage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTabForImage(QWidget *parent = 0);
    explicit CustomTabForImage(QWidget *parent = 0, QString graphicFilePath = "", QDir directoryWithResults = QDir());

    void loadFile();

    ~CustomTabForImage();

    QImage *getProcessedQImage();
    void setDirectoryWithResults(QDir directory);

private slots:
    void on_groundTruthCheckBox_toggled(bool checked);

    void on_calculatedCheckBox_toggled(bool checked);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    Ui::CustomTabForImage *ui;
    void updateImage();
    std::vector<Annotation> parseAnnotation(QDir directory);


    void setupQGraphicsViews();

    QImage *originalGrahpicImage = nullptr;
    QImage *withMarksGrahpicImage = nullptr;

    QGraphicsPixmapItem *grahpicPixmap;
    QGraphicsView *graphicView;
    QGraphicsScene *grahpicScane;

    QCheckBox *groundTruthCheckBox;
    QCheckBox *calculatedCheckBox;

    QString graphicFilePath;
    QFileInfo *graphicQFileInfo;
    QString fileName;
    QDir directoryWithGroundTruthAnnotations;
    QDir directoryWithResults;


    std::vector<Annotation> groundTruthLocations;
    std::vector<Annotation> calculatedLocations;
    const QString AnnotationExtension = ".pts";
    QString faceElements[20] = {"Prawe oko",
                              "Lewe oko",
                              "Prawy kącik ust",
                              "Lewy kącik ust",
                              "Zewnętrzy koniec prawej brwi",
                              "Wewnętrzny koniec prawej brwi",
                              "Wewnętrzny koniec lewej brwi",
                              "Zewnętrzy koniec lewej brwi",
                              "Prawa skroń",
                              "Zewnętrzny kącik prawego oka",
                              "Wewnętrzny kącik prawego oka",
                              "Wewnętrzny kącik lewego oka",
                              "Zewnętrzny kącik lewego oka",
                              "Lewa skroń",
                              "Nos",
                              "Prawe nozdrze",
                              "Lewe nozdrze",
                              "Górna warga",
                              "Dolna warga",
                              "Podbródek"
                             };
    const int faceElementsCount = 20;
    const int markSize = 2;
};

#endif // CUSTOMTABFORIMAGE_H
