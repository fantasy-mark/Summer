#include "summer.h"
#include <QApplication>

#include "xdev.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Summer w;
    w.show();

    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

    return a.exec();
}
