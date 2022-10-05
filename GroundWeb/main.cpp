#include "GroundWeb.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GroundWeb w;
    w.show();



    return a.exec();
}
