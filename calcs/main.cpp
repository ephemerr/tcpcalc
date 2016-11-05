#include <QCoreApplication>
#include <calcserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    CalcServer server(&a);

    return a.exec();
}
