#include "xconfig.h"
#include <QFile>
#include <qDebug>

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

bool XConfig::load_XML(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument dom("XConfig");
        if (dom.setContent(&file)) {
            QDomElement docElem = dom.documentElement();
            list_DOM(docElem);
        }
        file.close();

        return true;
    }

    return false;
}

void XConfig::list_DOM(QDomElement docElem)
{
    QDomNode node = docElem.firstChild();

    //叶子节点
    if(node.toElement().isNull()) {
        globalConfig[docElem.tagName()] = docElem.text();
    } else {
        while(! node.isNull()) {
            QDomElement element = node.toElement(); // try to convert the node to an element.
            //非叶子节点则遍历子节点
            list_DOM(element);
            node = node.nextSibling();
        }
    }
}
