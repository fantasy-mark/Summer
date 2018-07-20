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
