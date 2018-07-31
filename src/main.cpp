#include "summer.h"
#include <QApplication>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QObject>

#include "xconfig.h"
#include "xdev.h"
#include <QDebug>

void setup();
void set_configFile();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setup();

    Summer w;
    w.show();

    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

    return a.exec();
}

void setup()
{
    set_configFile();
}

void set_configFile()
{
    QStringList fileNames;
    QString path = QDir::currentPath() + "/../Summer/resource/config.xml";
    if (! XConfig::Get()->load_XML(path)) {
        QFileDialog * fileDlg = new QFileDialog();
        fileDlg->setWindowFilePath(QDir::currentPath());
        fileDlg->setWindowTitle(QStringLiteral("打开配置文件"));
        fileDlg->setNameFilter(QStringLiteral("DOM(*.xml)"));
        //can select only one file
        fileDlg->setFileMode(QFileDialog::ExistingFile);
        if (fileDlg->exec()) {
            fileNames = fileDlg->selectedFiles();
            if (! XConfig::Get()->load_XML(fileNames[0]))
                QMessageBox::warning(NULL, QStringLiteral("配置"), QStringLiteral("配置文件有误"));
        }
    }
    //qDebug() << XConfig::Get()->globalConfig;
}
