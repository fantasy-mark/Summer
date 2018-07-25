#include <qDebug>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include "summer.h"

#include "xdev.h"
#include "xserial.h"

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

void Summer::show_devList()
{
    if (irExamPage == NULL)
        return;

    irExamPage->CB1->clear();
    QStringList list = XDev::Get()->refresh_irDev();
    QList<QString>::Iterator it = list.begin(), itend = list.end();
    for (int i = 0; it != itend; it++, i++) {
        irExamPage->CB1->insertItem(i, *it);
    }

    return;
}

void Summer::connect_irDev()
{
    if (XDev::Get()->connect_irDev()) {
        irExamPage->irConnect->setEnabled(false);
        irExamPage->irDisconnect->setEnabled(true);
        irExamPage->Play->setEnabled(true);
        irExamPage->Pause->setEnabled(true);
        irExamPage->Photo->setEnabled(true);
        irExamPage->AutoFocus->setEnabled(true);
        irExamPage->NearFocus->setEnabled(true);
        irExamPage->FarFocus->setEnabled(true);
        irExamPage->CB2->setEnabled(true);
    } else {
        QMessageBox::information(this, QString(), QStringLiteral("无法连接红外探测头"));
    }
}

void Summer::disconnect_irDev()
{
    XDev::Get()->disconnect_irDev();
    irExamPage->irConnect->setEnabled(true);
    irExamPage->irDisconnect->setEnabled(false);
    irExamPage->Play->setEnabled(false);
    irExamPage->Pause->setEnabled(false);
    irExamPage->Photo->setEnabled(false);
    irExamPage->AutoFocus->setEnabled(false);
    irExamPage->NearFocus->setEnabled(false);
    irExamPage->FarFocus->setEnabled(false);
    irExamPage->CB2->setEnabled(false);
}

//=============================================================================
//=============================================================================
//=============================================================================

void Summer::create_userManagerPage()
{
    if (userManagerWidget != NULL) {
        if (current != userManagerWidget) {
            current->hide();
            userManagerWidget->show();
            current =  userManagerWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    userManagerWidget = new QWidget(this);
    userManagerWidget->move(0, 100);
    userManagerPage = new Ui::userManager;
    userManagerPage->setupUi(userManagerWidget);
    userManagerWidget->show();
    current = userManagerWidget;
}

void Summer::create_customerSearchPage()
{
    if (customerSearchWidget != NULL) {
        if (current != customerSearchWidget) {
            current->hide();
            customerSearchWidget->show();
            current =  customerSearchWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    customerSearchWidget = new QWidget(this);
    customerSearchWidget->move(0, 100);
    customerSearchPage = new Ui::customerSearch;
    customerSearchPage->setupUi(customerSearchWidget);
    customerSearchWidget->show();
    current = customerSearchWidget;
}

void Summer::create_baseInfoPage()
{
    if (baseInfoWidget != NULL) {
        if (current != baseInfoWidget) {
            current->hide();
            baseInfoWidget->show();
            current =  baseInfoWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    baseInfoWidget = new QWidget(this);
    baseInfoWidget->move(0, 100);
    baseInfoPage = new Ui::baseInfo;
    baseInfoPage->setupUi(baseInfoWidget);
    baseInfoWidget->show();
    current = baseInfoWidget;
}

void Summer::create_selfCheckPage()
{
    if (selfCheckWidget != NULL) {
        if (current != selfCheckWidget) {
            current->hide();
            selfCheckWidget->show();
            current =  selfCheckWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    selfCheckWidget = new QWidget(this);
    selfCheckWidget->move(0, 100);
    selfCheckPage = new Ui::selfCheck;
    selfCheckPage->setupUi(selfCheckWidget);
    selfCheckWidget->show();
    current = selfCheckWidget;
}

#define YaQian_HSV  11
void Summer::create_irExamPage()
{
    if (irExamWidget != NULL) {
        if (current != irExamWidget) {
            current->hide();
            irExamWidget->show();
            current =  irExamWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    irExamWidget = new QWidget(this);
    irExamWidget->move(0, 100);
    irExamPage = new Ui::irExam;
    irExamPage->setupUi(irExamWidget);
    irExamWidget->show();
    //irDev signal & slot
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(irExamPage->irRefresh, SIGNAL(clicked()), this, SLOT(show_devList()));
    connect(irExamPage->irConnect, SIGNAL(clicked()), this, SLOT(connect_irDev()));
    connect(irExamPage->irDisconnect, SIGNAL(clicked()), this, SLOT(disconnect_irDev()));
    connect(irExamPage->Play, SIGNAL(clicked()), XDev::Get(), SLOT(play_irDev()));
    connect(irExamPage->Pause, SIGNAL(clicked()), XDev::Get(), SLOT(stop_irDev()));
    connect(XDev::Get(), SIGNAL(magFrame(cv::Mat)), irExamPage->View, SLOT(play(cv::Mat)));
    connect(irExamPage->AutoFocus, SIGNAL(clicked()), XDev::Get(), SLOT(auto_focus()));
    connect(irExamPage->FarFocus, SIGNAL(clicked()), XDev::Get(), SLOT(far_focus()));
    connect(irExamPage->NearFocus, SIGNAL(clicked()), XDev::Get(), SLOT(near_focus()));
    //cradle & pole signal & slot
    connect(irExamPage->cradleUp, SIGNAL(pressed()), XDev::Get(), SLOT(up_cradle()));
    connect(irExamPage->cradleDown, SIGNAL(pressed()), XDev::Get(), SLOT(down_cradle()));
    connect(irExamPage->cradleUp, SIGNAL(released()), XDev::Get(), SLOT(stop_cradle()));
    connect(irExamPage->cradleDown, SIGNAL(released()), XDev::Get(), SLOT(stop_cradle()));

    show_devList();
    //显示色卡
    QString str = "Gray0to255|Gray255to0|IronBow|RainBow|GlowBow|Autumn|Winter|"
                  "HotMetal|Jet|RedSaturation|HighContrast|YaQian_HSV|Nice";
    irExamPage->CB2->insertItems(0, str.split("|"));
    irExamPage->CB2->setCurrentIndex(YaQian_HSV);
    connect(irExamPage->CB2, SIGNAL(currentIndexChanged(int)), irExamPage->View, SLOT(set_colormap(int)));

    current = irExamWidget;
}

void Summer::create_assessReportPage()
{
    if (assessReportWidget != NULL) {
        if (current != assessReportWidget) {
            current->hide();
            assessReportWidget->show();
            current =  assessReportWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    assessReportWidget = new QWidget(this);
    assessReportWidget->move(0, 100);
    assessReportPage = new Ui::assessReport;
    assessReportPage->setupUi(assessReportWidget);
    assessReportWidget->show();
    current = assessReportWidget;
}

void Summer::create_recuperatePlanPage()
{
    if (recuperatePlanWidget != NULL) {
        if (current != recuperatePlanWidget) {
            current->hide();
            recuperatePlanWidget->show();
            current =  recuperatePlanWidget;
        }
        return;
    }

    if (current != NULL)
        current->hide();

    recuperatePlanWidget = new QWidget(this);
    recuperatePlanWidget->move(0, 100);
    recuperatePlanPage = new Ui::recuperatePlan;
    recuperatePlanPage->setupUi(recuperatePlanWidget);
    recuperatePlanWidget->show();
    current = recuperatePlanWidget;
}

//=============================================================================
//=============================================================================
//=============================================================================
