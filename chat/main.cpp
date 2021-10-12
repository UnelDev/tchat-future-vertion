#include "userinterface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    userinterface w;
    w.show();
    return a.exec();
}
