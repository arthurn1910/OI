#include <QCoreApplication>
#include <QDebug>

#include <iostream>
#include <processor.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc != 4)
    {
        qDebug() << "Invalid number of arguments";
        return 1;
    }

    Processor * processor = new Processor();
    if (processor->process(std::atoi(argv[1]), argv[2], argv[3]) == 1)
    {
        return 0;
    }

    return 0;
}
