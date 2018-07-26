#include "xconfig.h"
#include <QFile>

XConfig::XConfig()
{

}

XConfig::~XConfig()
{

}

XConfig * XConfig::Get()
{
    static XConfig xc;
    return &xc;
}

void XConfig::read_xml(QString path)
{
    QDomDocument doc("mydocument");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            globalConfig[e.tagName()] = e.text();
        }
        n = n.nextSibling();
    }
}
