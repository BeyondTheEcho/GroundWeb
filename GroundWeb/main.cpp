#include "GroundWeb.h"
#include "NetworkManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    GroundWeb w;
    NetworkManager net(&w);
    w.show();

    return a.exec();
}
