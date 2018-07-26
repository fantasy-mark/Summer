#include "summer.h"
#include <QApplication>

#include "xconfig.h"
#include "xdev.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XConfig::Get()->read_xml(QDir::currentPath() + "/../resource/config.xml");

    Summer w;
    w.show();

    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

    return a.exec();
}
