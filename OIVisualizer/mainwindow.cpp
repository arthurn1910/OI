#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customtabforimage.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <iostream>
#include <QStandardPaths>
#include <QImageWriter>
#include <QImageReader>
#include <QErrorMessage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionFile_triggered()
{
    QString newFilePath = this->selectNewFile();

    if (newFilePath != NULL) {
        CustomTabForImage *newTab = new CustomTabForImage(ui->tabWidget, newFilePath);
        ui->tabWidget->addTab(newTab, newFilePath.split("/").last());
        ui->tabWidget->setCurrentWidget(newTab);
    }
}

void MainWindow::initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    dialog.setDirectory(lastDirectory.isEmpty() ? QDir::currentPath() : lastDirectory);

    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("pgm");
}

QString MainWindow::selectNewFile()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    if (dialog.exec()) {
        lastDirectory = dialog.selectedFiles().first();
        return lastDirectory;
    } else {
        return NULL;
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionKatalog_z_wynikiem_test_w_triggered()
{
    directoryWithResults = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                             lastDirectory.isEmpty() ? QDir::currentPath() : lastDirectory,
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);
    for (int i = 0; i < ui->tabWidget->count(); i++) {
        CustomTabForImage *tab = (CustomTabForImage *) (ui->tabWidget->widget(i));
        tab->setDirectoryWithResults(directoryWithResults);
    }
}
