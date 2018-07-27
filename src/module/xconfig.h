#pragma once

#include <QMap>
#include <QObject>
#include <QDomDocument>
#include <QTreeWidgetItem>

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.26
    Description	: ≈‰÷√Œƒº˛
 *****************************************************************************/
class XConfig : public QObject
{
    Q_OBJECT

public:
    ~XConfig();
    static XConfig * Get();
    QMap<QString, QString> globalConfig = {};

    bool load_XML(QString fileName);
    void list_DOM(QDomElement docElem);

private:
    XConfig();
};
