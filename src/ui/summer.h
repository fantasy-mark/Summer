#ifndef SUMMER_H
#define SUMMER_H

#include <QWidget>

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

public slots:
    void create_userManagerPage();
    void create_customerSearchPage();
    void create_baseInfoPage();
    void create_selfCheckPage();
    void create_irExamPage();
    void create_assessReportPage();
    void create_recuperatePlanPage();
    void show_devList();
    void connect_irDev();
    void disconnect_irDev();
    void play_irDev();

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
    Ui::assessReport * assessReportPage;
    QWidget * assessReportWidget = nullptr;
    Ui::recuperatePlan * recuperatePlanPage;
    QWidget * recuperatePlanWidget = nullptr;
};

#endif // SUMMER_H
