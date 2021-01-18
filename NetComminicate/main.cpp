#include "netcomminicate.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetComminicate w;
    w.show();
    return a.exec();
}
