#include "mmainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logo.svg"));

    MMainWindow w;
    w.setMinimumSize(600,480);
    w.moveToCenter();
    w.show();

    return a.exec();
}
