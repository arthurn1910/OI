#include "customtabforimage.h"
#include "ui_customtabforimage.h"
#include <QGraphicsView>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <string>
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <QCursor>
#include <QDebug>
#include "cmath"

CustomTabForImage::CustomTabForImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomTabForImage)
{
    ui->setupUi(this);
    groundTruthCheckBox = ui->groundTruthCheckBox;
    calculatedCheckBox = ui->calculatedCheckBox;
}

CustomTabForImage::CustomTabForImage(QWidget *parent, QString graphicFilePath, QDir directoryWithResults) :
    QWidget(parent),
    ui(new Ui::CustomTabForImage)
{
    ui->setupUi(this);

    groundTruthCheckBox = ui->groundTruthCheckBox;
    calculatedCheckBox = ui->calculatedCheckBox;
    this->directoryWithResults = directoryWithResults;

    this->graphicFilePath = graphicFilePath;
    graphicQFileInfo = new QFileInfo(graphicFilePath);
    fileName = graphicQFileInfo->baseName();
    directoryWithGroundTruthAnnotations = QDir(graphicQFileInfo->absolutePath());
    directoryWithGroundTruthAnnotations.cdUp();
    directoryWithGroundTruthAnnotations.cd("annotations");
    groundTruthLocations = parseAnnotation(directoryWithGroundTruthAnnotations);

    setupQGraphicsViews();
    loadFile();
}

CustomTabForImage::~CustomTabForImage()
{
    delete ui;
}

void CustomTabForImage::setupQGraphicsViews()
{
    graphicView = ui->originalGrahpic;
    grahpicScane = new QGraphicsScene();
    grahpicPixmap = new QGraphicsPixmapItem();
    grahpicScane->addItem(grahpicPixmap);
    grahpicScane->installEventFilter(this);
    graphicView->setScene(grahpicScane);
    graphicView->setMouseTracking(true);
    graphicView->show();

}

void CustomTabForImage::loadFile() {
    originalGrahpicImage = new QImage(graphicFilePath);
    withMarksGrahpicImage = new QImage(originalGrahpicImage->width(), originalGrahpicImage->height(), QImage::Format_RGB888);
    for (int i = 0; i < withMarksGrahpicImage->height(); i++) {
        for (int j = 0; j < withMarksGrahpicImage->width(); j++) {
            int pixelValue = originalGrahpicImage->pixel(j, i);
            withMarksGrahpicImage->setPixel(j, i, qRgb(pixelValue, pixelValue, pixelValue));
        }
    }

    grahpicPixmap->setPixmap(QPixmap::fromImage(*withMarksGrahpicImage));
}

QImage *CustomTabForImage::getProcessedQImage()
{
    return withMarksGrahpicImage;
}

bool CustomTabForImage::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() ==  QEvent::GraphicsSceneMouseMove ) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);

    QPointF img_coord_pt = mouseEvent->scenePos();

    double x = img_coord_pt.x();
    double y = img_coord_pt.y();

    if (groundTruthCheckBox->isChecked()) {
        for (int i = 0; i < groundTruthLocations.size(); i++) {
            Annotation currentAnnotationGroundTruth = groundTruthLocations.at(i);
            if (std::pow(currentAnnotationGroundTruth.x - x, 2) + std::pow(currentAnnotationGroundTruth.y - y, 2) <= std::pow(2 * markSize, 2)) {
                QToolTip::showText(QCursor::pos(), currentAnnotationGroundTruth.label);
            } else {
                QToolTip::hideText();
            }
        }
    }

    if (calculatedCheckBox->isChecked()) {
        for (int i = 0; i < calculatedLocations.size(); i++) {
            Annotation currentAnnotationCalculated = calculatedLocations.at(i);
            if (std::pow(currentAnnotationCalculated.x - x, 2) + std::pow(currentAnnotationCalculated.y - y, 2) <= std::pow(2 * markSize, 2)) {
                QToolTip::showText(QCursor::pos(), currentAnnotationCalculated.label);
            } else {
                QToolTip::hideText();
            }
        }
    }

    return true;
  } else {
    return QObject::eventFilter(obj, event);
  }
}

void CustomTabForImage::setDirectoryWithResults(QDir directory) {
    this->directoryWithResults = directory;
    calculatedLocations = parseAnnotation(directoryWithResults);
    updateImage();
}

void CustomTabForImage::updateImage() {
    for (int i = 0; i < withMarksGrahpicImage->height(); i++) {
        for (int j = 0; j < withMarksGrahpicImage->width(); j++) {
            int pixelValue = originalGrahpicImage->pixel(j, i);
            withMarksGrahpicImage->setPixel(j, i, qRgb(pixelValue, pixelValue, pixelValue));
        }
    }

    if (groundTruthCheckBox->isChecked()) {
        for (int i = 0; i < groundTruthLocations.size(); i++) {
            Annotation currentAnnotation = groundTruthLocations.at(i);
            for (int x = -markSize; x < markSize; x++) {
                for (int y = -markSize; y < markSize; y++) {
                    if (std::pow(x, 2) + std::pow(y, 2) <= std::pow(markSize, 2)) {
                        withMarksGrahpicImage->setPixel(currentAnnotation.x + x, currentAnnotation.y + y, qRgb(255, 0, 0));
                    }
                }
            }
        }
    }

    if (calculatedCheckBox->isChecked()) {
        for (int i = 0; i < calculatedLocations.size(); i++) {
            Annotation currentAnnotation = calculatedLocations.at(i);
            for (int x = -markSize; x < markSize; x++) {
                for (int y = -markSize; y < markSize; y++) {
                    if (std::pow(x, 2) + std::pow(y, 2) <= std::pow(markSize, 2)) {
                        withMarksGrahpicImage->setPixel(currentAnnotation.x + x, currentAnnotation.y + y, qRgb(0, 0, 255));
                    }
                }
            }
        }
    }
    grahpicPixmap->setPixmap(QPixmap::fromImage(*withMarksGrahpicImage));
    graphicView->repaint();
}

std::vector<Annotation> CustomTabForImage::parseAnnotation(QDir directory) {
    QFile file(directory.absolutePath() + QDir::separator() + fileName + AnnotationExtension);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    // Skipping heading
    in.readLine();
    in.readLine();
    in.readLine();
    std::vector<Annotation> parsedAnnotations;
    for (int i = 0; i < faceElementsCount; i++) {
        QString line = in.readLine();
        QStringList positions = line.split(QRegExp("\\s"));
        Annotation parsedAnnotation;
        parsedAnnotation.x = positions[0].toDouble();
        parsedAnnotation.y = positions[1].toDouble();
        parsedAnnotation.label = faceElements[i];

        parsedAnnotations.push_back(parsedAnnotation);
    }

    file.close();

    return parsedAnnotations;
}

void CustomTabForImage::on_groundTruthCheckBox_toggled(bool checked)
{
    updateImage();
}

void CustomTabForImage::on_calculatedCheckBox_toggled(bool checked)
{
    updateImage();
}
