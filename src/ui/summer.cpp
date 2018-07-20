#include <qDebug>
#include <QProcess>
#include <QInputDialog>
#include "summer.h"
#include "ui_summer.h"

Summer::Summer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Summer)
{
    ui->setupUi(this);
}

Summer::~Summer()
{
    delete ui;
}

void Summer::git_create()
{
    QProcess * po = new QProcess(this);
    QStringList argv;
    argv.append("init");

    po->start("git.exe", argv);
}

void Summer::git_clone()
{
    QString projectName = "Summer";
    QProcess * po = new QProcess(this);
    QInputDialog * dlg = new QInputDialog(this);
    dlg->setLabelText("input project name");
    dlg->setTextValue("Summer");
    dlg->show();
    if(dlg->exec() == QDialog::Accepted) {
        projectName = dlg->textValue();
    }
    QStringList argv;
    argv.append("clone");
    QString tmp = "https://github.com/fantasy-mark/" + projectName + ".git";
    argv.append(tmp);
    ui->l2->setText(QString("git clone ") + tmp);

    po->start("git.exe", argv);
}

void Summer::git_pull()
{
    QProcess * po = new QProcess(this);
    QStringList argv;
    argv.append("pull");
    argv.append("origin");
    argv.append("master");

    po->start("git.exe", argv);
}

void Summer::git_add()
{
    QProcess * po = new QProcess(this);
    QStringList argv;
    argv.append("add");
    argv.append("*");

    po->start("git.exe", argv);
}

void Summer::git_commit()
{
    QProcess * po = new QProcess(this);
    QStringList argv;
    QInputDialog * dlg = new QInputDialog(this);
    QString commitInfo;
    dlg->setLabelText("input commit info");
    dlg->show();
    if(dlg->exec() == QDialog::Accepted) {
        commitInfo = dlg->textValue();
    }
    argv.append("commit");
    argv.append("-m");
    argv.append(commitInfo);

    po->start("git.exe", argv);
}

void Summer::git_addRemote()
{
    QProcess * po = new QProcess(this);
    QStringList argv;
    argv.append("remote");
    argv.append("add");
    argv.append("origin");
    QString projectName;
    QInputDialog * dlg = new QInputDialog(this);
    dlg->setLabelText("input project name");
    dlg->setTextValue("Summer");
    dlg->show();
    if(dlg->exec() == QDialog::Accepted) {
        projectName = dlg->textValue();
    }
    QString tmp = "https://github.com/fantasy-mark/" + projectName + ".git";
    argv.append(tmp);

    po->start("git.exe", argv);
}

void Summer::git_push()
{
    QProcess * po = new QProcess(this);
    QStringList argv;

    argv.append("push");
    argv.append("-u");
    argv.append("origin");
    argv.append("master");

    po->start("git.exe", argv);
}
