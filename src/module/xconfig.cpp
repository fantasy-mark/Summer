#include <QDir>
#include <QFile>
#include <qDebug>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>

#include "xconfig.h"

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.14
    Description	: 获取Xconfig实例
 *****************************************************************************/
XConfig * XConfig::Get()
{
    static XConfig xc;
    return &xc;
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.14
    Description	: 配置config文件
 *****************************************************************************/
bool XConfig::setup()
{
    QString path = QDir::currentPath() + "/config.json";

    QFile file(path);

    if(! file.open(QIODevice::ReadOnly)) {
        QFileDialog * fileDlg = new QFileDialog();
        //can select only one file
        fileDlg->setFileMode(QFileDialog::ExistingFile);
        fileDlg->setWindowFilePath(QDir::currentPath());
        fileDlg->setWindowTitle(QStringLiteral("未找到 config.json"));
        fileDlg->setNameFilter(QStringLiteral("json文件(*.json)"));
        if (fileDlg->exec()) {
            path = fileDlg->selectedFiles().at(0);
            file.setFileName(path);
            if(! file.open(QIODevice::ReadOnly)) {
                QMessageBox::warning(NULL, QStringLiteral("json文件"), QStringLiteral("文件格式有误"));
                return false;
            }
        } else {
            return false;
        }
    }

    QByteArray allData = file.readAll();
    file.close();

    xconfig = QJsonDocument::fromJson(allData).object();

    return true;
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 获取对应配置项值
 *****************************************************************************/
QString XConfig::value(const char * key)
{
    QJsonValue data = xconfig.value(QString(key));

    if (data.isString()) {
        return data.toString();
    } else if (data.isDouble()) {
        return QString::number(data.toDouble());
    } else {
        return "Not define!";
    }

}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.14
    Description	: 配置保存json文件
 *****************************************************************************/
void XConfig::configure()
{
    tree = new QTreeWidget;
    QStringList labels;
    labels << QStringLiteral("标签/键") << QStringLiteral("值");
    tree->setHeaderLabels(labels);

    QStringList cols;

    QStringList l = xconfig.keys();
    foreach (QString k, l) {
        QJsonValue data = xconfig.value(k);

        if (data.isString()) {
            cols.clear();
            cols << k << data.toString();
            new QTreeWidgetItem(tree, cols);
        } else if (data.isDouble()) {
            cols.clear();
            cols << k << QString::number(data.toDouble());
            new QTreeWidgetItem(tree, cols);
        } else if (data.isObject()) {
            cols.clear();
            cols << k;
            QTreeWidgetItem * li = new QTreeWidgetItem(tree, cols);
            QStringList l2 = data.toObject().keys();
            foreach (QString k, l2) {
                QJsonValue data2 = data.toObject().value(k);
                if (data2.isString()) {
                    cols.clear();
                    cols << "\t" + k << data2.toString();
                    QTreeWidgetItem * chiled = new QTreeWidgetItem(tree, cols);
                    li->addChild(chiled);
                } else if (data2.isDouble()) {
                    cols.clear();
                    cols << "\t" + k << QString::number(data2.toDouble());
                    QTreeWidgetItem * chiled = new QTreeWidgetItem(tree, cols);
                    li->addChild(chiled);
                }
            }
        }
    }
    tree->show();
}
