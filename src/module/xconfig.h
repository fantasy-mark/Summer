#pragma once

#include <QMap>
#include <QObject>
#include <QDomDocument>

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
    QMap<QString, QString> globalConfig = {};

    ~XConfig();
    static XConfig * Get();
    void read_xml(QString);

private:
    XConfig();
};
