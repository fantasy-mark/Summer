#ifndef SUMMER_H
#define SUMMER_H

#include <QWidget>
#include <QSystemTrayIcon>

#include "ui_t3l0.h"
#include "ui_t3l1.h"
#include "ui_t3l2.h"
#include "ui_t3l3.h"
#include "ui_t3l4.h"
#include "ui_t3l5.h"
#include "ui_t3l6.h"
#include "ui_t3l7.h"
#include "ui_t3l8.h"
#include "ui_t3l9.h"
#include "ui_t3l10.h"
#include "ui_t3l11.h"
#include "ui_t3l12.h"

#include "ui_userManager.h"
#include "ui_customerSearch.h"
#include "ui_baseInfo.h"
#include "ui_selfCheck.h"
#include "ui_irExam.h"
#include "ui_assessReport.h"
#include "ui_recuperatePlan.h"

namespace Ui {
class Summer;
}

class Summer : public QWidget
{
    Q_OBJECT

public:
    explicit Summer(QWidget *parent = 0);
    ~Summer();

    QListWidget * tabListWidget = NULL;
    QSystemTrayIcon * mSysTrayIcon = NULL;

    Mat editorMat;

public slots:
    void get_baseInfo();
    void get_selfCheck();
    void print_report(void);

    void create_userManagerPage();
    void create_customerSearchPage();
    void create_baseInfoPage();
    void create_selfCheckPage();
    void create_irExamPage();
    void create_assessReportPage();
    void create_recuperatePlanPage();

    void show_devList();
    void show_editorMat();

    void connect_irDev();
    void disconnect_irDev();
    void closeEvent(QCloseEvent * event);

    void verify();
    void commit_baseInfo();
    void commit_selfCheck();
    void commit_irExam();

#ifdef USING_GTEST
    void run_gTest();
#endif
    void minimum();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Summer *ui;
    QWidget * current = NULL;

    Ui::userManager * userManagerPage;
    QWidget * userManagerWidget = nullptr;
    Ui::customerSearch * customerSearchPage;
    QWidget * customerSearchWidget = nullptr;
    Ui::baseInfo * baseInfoPage;
    QWidget * baseInfoWidget = nullptr;
    Ui::selfCheck * selfCheckPage;
    QWidget * selfCheckWidget = nullptr;
    Ui::irExam * irExamPage;
    QWidget * irExamWidget = nullptr;
    Ui::assessReport * assessReportPage = nullptr;
    QWidget * assessReportWidget = nullptr;
    Ui::recuperatePlan * recuperatePlanPage;
    QWidget * recuperatePlanWidget = nullptr;

    Ui::T3L0 * t3l0;
    Ui::T3L1 * t3l1;
    Ui::T3L2 * t3l2;
    Ui::T3L3 * t3l3;
    Ui::T3L4 * t3l4;
    Ui::T3L5 * t3l5;
    Ui::T3L6 * t3l6;
    Ui::T3L7 * t3l7;
    Ui::T3L8 * t3l8;
    Ui::T3L9 * t3l9;
    Ui::T3L10 * t3l10;
    Ui::T3L11 * t3l11;
    Ui::T3L12 * t3l12;

protected:
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // SUMMER_H
