#include "calcx.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calcx w;
    w.show();

    return a.exec();
}
