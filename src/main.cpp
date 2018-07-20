#include "summer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Summer w;
    w.show();

    return a.exec();
}
