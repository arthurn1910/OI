#include <QCoreApplication>
#include <picture.h>
#include <qDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString openPath = "C:/Users/A638852/Documents/sf/a";//argv[1];
    //solutionPath = //argv[2];
    QString savePath ="C:/Users/A638852/Documents/sf/tt"; //argv[3];
    QString id;
    Picture *picture[1523];
    for(int i=0;i<=1523;i++){
        qDebug()<<QString::number(i);
        if(i<10){
            id="000"+QString::number(i);
        }else if(i<100){
            id="00"+QString::number(i);
        }else if(i<1000){
            id="0"+QString::number(i);
        }else{
            id=QString::number(i);
        }
        qDebug()<<id;

        picture[i]=new Picture(id,openPath,savePath);
        picture[i]->read();
        picture[i]->save();
    }

    return a.exec();
}
