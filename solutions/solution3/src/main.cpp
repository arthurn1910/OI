#include <QCoreApplication>
#include <QDebug>

#include <processor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc != 4)
    {
        qDebug() << "Invalid number of arguments";
        return 1;
    }

    Processor* processor = new Processor(std::atoi(argv[1]), argv[2], argv[3]);
    processor->process();

    return 0;
}
