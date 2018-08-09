#include <qDebug>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QFormLayout>
#include <QTextStream>
#include <QScrollBar>
#include <QPalette>
#include "summer.h"

#include "xdev.h"
#include "xserial.h"
#include "xreport.h"
#include "xconfig.h"

#include "gtest/gtest.h"

#include "ui_summer.h"

Summer::Summer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Summer)
{
    ui->setupUi(this);
    create_userManagerPage();

    ui->label->hide();
    ui->label_2->hide();
    ui->commit->hide();

    XReport::Get()->create_Report();

    connect(ui->gtest, SIGNAL(clicked()), this, SLOT(run_gTest()));
}

Summer::~Summer()
{
    delete ui;
}

void Summer::run_gTest()
{
    RUN_ALL_TESTS();
}

void Summer::minimum()
{
    //hide 隐藏程序主窗口,在状态栏不会显示 minimized,需要icon双击打开 | 只是最小化窗口,无需从icon去双击打开显示
    //this->hide();
    showMinimized();

    //新建QSystemTrayIcon对象
    if (mSysTrayIcon == NULL) {
        mSysTrayIcon = new QSystemTrayIcon(this);
        //新建托盘要显示的icon
        QIcon icon = QIcon("C:/rrk/test.png");
        //将icon设到QSystemTrayIcon对象中
        mSysTrayIcon->setIcon(icon);
        //当鼠标移动到托盘上的图标时，会显示此处设置的内容
        mSysTrayIcon->setToolTip(QStringLiteral("乳腺诊断仪"));
        //给QSystemTrayIcon添加槽函数
        connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    } else {
        //在系统托盘显示此对象
        mSysTrayIcon->show();
    }
}

void Summer::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示主程序窗口
        //this->show();
        showNormal();
        //this->setVisible(true);
        break;
    default:
        break;
    }
}

void Summer::show_devList()
{
    irExamPage->CB1->clear();
    QStringList list = XDev::Get()->refresh_irDev();
    qDebug() << list;
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
        irExamPage->Photo_2->setEnabled(true);
        irExamPage->AutoFocus->setEnabled(true);
        irExamPage->NearFocus->setEnabled(true);
        irExamPage->FarFocus->setEnabled(true);
        irExamPage->CB2->setEnabled(true);
        //XDev::Get()->play_irDev();
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
    irExamPage->Photo_2->setEnabled(false);
    irExamPage->AutoFocus->setEnabled(false);
    irExamPage->NearFocus->setEnabled(false);
    irExamPage->FarFocus->setEnabled(false);
    irExamPage->CB2->setEnabled(false);
}

void Summer::closeEvent(QCloseEvent * event)
{
    if (assessReportPage != NULL)
        assessReportPage->reportView->deleteLater();
    this->close();
}

//=============================================================================
//=============================================================================
//=============================================================================
void Summer::verify()
{
    if (userManagerPage != NULL) {
        if (userManagerPage->password->text() == "pass")
            create_baseInfoPage();
    }
}

void Summer::create_userManagerPage()
{
    ui->label->hide();
    ui->label_2->hide();
    ui->commit->hide();

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
    userManagerWidget->move(0, 73);
    userManagerPage = new Ui::userManager;
    userManagerPage->setupUi(userManagerWidget);

    QWidget * formLayoutWidget = new QWidget(this);
    QFormLayout * formLayout = new QFormLayout(formLayoutWidget);

    formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
    formLayoutWidget->setGeometry(QRect(110, 400, 371, 231));

    formLayout->setObjectName(QStringLiteral("formLayout"));
    formLayout->setContentsMargins(0, 0, 0, 0);

    //TODO 用于配置文件读取
#if 0
    int i = 0;
    QMap<QString, QString>::iterator item = XConfig::Get()->globalConfig.begin();
    while (item != XConfig::Get()->globalConfig.end()) {
        qDebug() << item.key() << item.value();
        QLabel * label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setText(item.key());

        formLayout->setWidget(i, QFormLayout::LabelRole, label);

        QLineEdit * lineEdit = new QLineEdit(formLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setText(item.value());

        formLayout->setWidget(i, QFormLayout::FieldRole, lineEdit);

        i++;
        item++;
    }
    formLayoutWidget->show();
#endif

#if 0
    QFile xmlfile("c:/rrk/test.xml");
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
        file.close();
        return;
    }

    QFile xmlfile("c:/rrk/test.xml");
    if (xmlfile.open(QFile::WriteOnly | QFile::Text)){
        //Dom方式写xml文件
        QTextStream out(&xmlfile);
        domDocument.save(out, QDomNode::EncodingFromDocument);
        xmlfile.close();
    }
#endif
    connect(userManagerPage->login, SIGNAL(clicked()), this, SLOT(verify()));

    userManagerWidget->show();
    current = userManagerWidget;
}

void Summer::create_customerSearchPage()
{
    ui->label->hide();
    ui->label_2->hide();
    ui->commit->hide();

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
    customerSearchWidget->move(0, 73);
    customerSearchPage = new Ui::customerSearch;
    customerSearchPage->setupUi(customerSearchWidget);
    customerSearchWidget->show();
    current = customerSearchWidget;
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.10
    Description	: 获取基本信息 & 槽函数
 *****************************************************************************/
void Summer::commit_baseInfo(void)
{
    if (current == baseInfoWidget) {
        get_baseInfo();
        XReport::Get()->create_BIReport();
        create_selfCheckPage();
    }
}

void Summer::commit_selfCheck(void)
{
    if (current == selfCheckWidget) {
        get_selfCheck();
        XReport::Get()->create_SCReport();
        create_irExamPage();
    }
}

void Summer::commit_irExam(void)
{
    if (current == irExamWidget) {
        create_assessReportPage();
    }
}

void Summer::get_baseInfo(void)
{
    //客户信息
    XReport::Get()->itemMap["name"] = baseInfoPage->name->text();
    XReport::Get()->itemMap["sex"] = baseInfoPage->sex->text();
    XReport::Get()->itemMap["age"] = baseInfoPage->age->text();
    XReport::Get()->itemMap["tel"] = baseInfoPage->tel->text();
    XReport::Get()->itemMap["id"] = baseInfoPage->id->text();
    //基本检查信息
    XReport::Get()->itemMap["height"] = baseInfoPage->height->text();
    XReport::Get()->itemMap["weight"] = baseInfoPage->weight->text();
    XReport::Get()->itemMap["bmi"] = baseInfoPage->bmi->text();
    XReport::Get()->itemMap["temp"] = baseInfoPage->temp->text();
    XReport::Get()->itemMap["fatR"] = baseInfoPage->fatR->text();
    XReport::Get()->itemMap["subFatR"] = baseInfoPage->subFatR->text();
    XReport::Get()->itemMap["viscusFatR"] = baseInfoPage->subFatR->text();
    XReport::Get()->itemMap["metabolicR"] = baseInfoPage->metabolicR->text();
    XReport::Get()->itemMap["boneMass"] = baseInfoPage->boneMass->text();
    XReport::Get()->itemMap["bodyMoisture"] = baseInfoPage->bodyMoisture->text();
    XReport::Get()->itemMap["bodyAge"] = baseInfoPage->bodyAge->text();
    XReport::Get()->itemMap["bloodSugar"] = baseInfoPage->bloodSugar->text();
    //脉诊信息
    XReport::Get()->itemMap["wristPulseL"] = baseInfoPage->wristPulseL->text();
    XReport::Get()->itemMap["wristPulseLH"] = baseInfoPage->wristPulseLH->text();
    XReport::Get()->itemMap["wristPulseLL"] = baseInfoPage->wristPulseLL->text();
    XReport::Get()->itemMap["wristPulseR"] = baseInfoPage->wristPulseR->text();
    XReport::Get()->itemMap["wristPulseRH"] = baseInfoPage->wristPulseRH->text();
    XReport::Get()->itemMap["wristPulseRL"] = baseInfoPage->wristPulseRL->text();
    XReport::Get()->itemMap["armPulseL"] = baseInfoPage->armPulseL->text();
    XReport::Get()->itemMap["armPulseLH"] = baseInfoPage->armPulseLH->text();
    XReport::Get()->itemMap["armPulseLL"] = baseInfoPage->armPulseLL->text();
    XReport::Get()->itemMap["armPulseR"] = baseInfoPage->armPulseR->text();
    XReport::Get()->itemMap["armPulseRH"] = baseInfoPage->armPulseRH->text();
    XReport::Get()->itemMap["armPulseRL"] = baseInfoPage->armPulseRL->text();
}


/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.13
    Description	: 获得QCheckBox勾选项文本
 *****************************************************************************/
template <typename T>
QString get_checkedText(T item)
{
    if (item->isChecked())
        return item->text() + ", ";
    else
        return "";
}


/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.11
    Description	: 获取自查页信息 & 槽函数
 *****************************************************************************/
void Summer::get_selfCheck(void)
{
    //家族病史
    QString familyHistory;
    QList<QCheckBox *> itemList0 = { t3l0->s11, t3l0->s12, t3l0->s13, t3l0->s14, t3l0->s15, t3l0->s16,
        t3l0->s21, t3l0->s22, t3l0->s23, t3l0->s24, t3l0->s25, t3l0->s26 };
    foreach(QCheckBox * item, itemList0) {
        familyHistory += get_checkedText(item);
    }
    if (! t3l0->s32->text().simplified().isEmpty()) {
        familyHistory += t3l0->s31->text();
        familyHistory += t3l0->s32->text();
    }
    XReport::Get()->itemMap["familyHistory"] = familyHistory;
    //过敏史
    QString allergicHistory;
    if (! t3l1->s11->isChecked()) {
        if (!t3l1->s13->text().simplified().isEmpty() || !t3l1->s15->text().simplified().isEmpty()) {
            allergicHistory += t3l1->s12->text();
            allergicHistory += t3l1->s13->text();
            allergicHistory += " " + t3l1->s14->text();
            allergicHistory += t3l1->s15->text();
        }
    }
    XReport::Get()->itemMap["allergicHistory"] = allergicHistory;
    //过往病史
    QString pastHistory;
    QList<QCheckBox *> itemList2 = { t3l2->s11, t3l2->s12, t3l2->s13, t3l2->s14, t3l2->s15, t3l2->s16,
        t3l2->s21, t3l2->s22, t3l2->s23, t3l2->s24, t3l2->s25, t3l2->s26,
        t3l2->s31, t3l2->s32, t3l2->s33, t3l2->s34, t3l2->s35, t3l2->s36 };
    foreach(QCheckBox * item, itemList2) {
        pastHistory += get_checkedText(item);
    }
    if (! t3l2->s42->text().simplified().isEmpty()) {
        pastHistory += t3l2->s41->text();
        pastHistory += t3l2->s42->text();
    }
    XReport::Get()->itemMap["pastHistory"] = pastHistory;
    //服药过敏史
    QString drugHistory;
    if (! t3l3->s12->text().simplified().isEmpty()) {
        drugHistory += t3l3->s11->text();
        drugHistory += t3l3->s12->text();
    }
    if (! t3l3->s14->text().simplified().isEmpty()) {
        drugHistory += t3l3->s13->text();
        drugHistory += t3l3->s14->text();
    }
    if (! t3l3->s16->text().simplified().isEmpty()) {
        drugHistory += t3l3->s15->text();
        drugHistory += t3l3->s16->text();
    }
    XReport::Get()->itemMap["drugHistory"] = drugHistory;
    //外伤史
    QString hurtHistory;
    if (! t3l4->s13->text().simplified().isEmpty()) {
        hurtHistory += t3l4->s12->text();
        hurtHistory += t3l4->s13->text();
    }
    XReport::Get()->itemMap["hurtHistory"] = hurtHistory;
    //手术史
    QString surgeryHistory;
    if (! t3l5->s13->text().simplified().isEmpty()) {
        surgeryHistory += t3l5->s12->text();
        surgeryHistory += t3l5->s13->text();
    }
    XReport::Get()->itemMap["surgeryHistory"] = surgeryHistory;
    //乳腺症状
    QString breastSymptom;
    QList<QCheckBox *> itemList6 = { t3l6->s11, t3l6->s12, t3l6->s21, t3l6->s22, t3l6->s31, t3l6->s32,
        t3l6->s41, t3l6->s42, t3l6->s51, t3l6->s61, t3l6->s71, t3l6->s72, t3l6->s81, t3l6->s82, t3l6->s91 };
    foreach(QCheckBox * item, itemList6) {
        breastSymptom += get_checkedText(item);
    }
    XReport::Get()->itemMap["breastSymptom"] = breastSymptom;
    //生活习惯
    QString lifeHabit;
    QList<QCheckBox *> itemList7_1 = { t3l7->s12, t3l7->s13, t3l7->s14, t3l7->s15 };
    foreach(QCheckBox * item, itemList7_1) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_2 = { t3l7->s22, t3l7->s23, t3l7->s24 };
    foreach(QCheckBox * item, itemList7_2) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_3 = { t3l7->s32, t3l7->s33, t3l7->s34 };
    foreach(QCheckBox * item, itemList7_3) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_4 = { t3l7->s42, t3l7->s43, t3l7->s44, t3l7->s45 };
    foreach(QCheckBox * item, itemList7_4) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_5 = { t3l7->s52, t3l7->s53, t3l7->s54 };
    foreach(QCheckBox * item, itemList7_5) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_6 = { t3l7->s62, t3l7->s63, t3l7->s64, t3l7->s65, t3l7->s66,
        t3l7->s67, t3l7->s72, t3l7->s73, t3l7->s74, t3l7->s75};
    foreach(QCheckBox * item, itemList7_6) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_8 = { t3l7->s82, t3l7->s83, t3l7->s84 };
    foreach(QCheckBox * item, itemList7_8) {
        lifeHabit += get_checkedText(item);
    }
    QList<QCheckBox *> itemList7_9 = { t3l7->s92, t3l7->s93, t3l7->s94 };
    foreach(QCheckBox * item, itemList7_9) {
        lifeHabit += get_checkedText(item);
    }
    XReport::Get()->itemMap["lifeHabit"] = lifeHabit;
    //经络辨识
    QString channelDistinguish;
    QList<QCheckBox *> itemList8 = { t3l8->s11, t3l8->s12, t3l8->s13, t3l8->s14, t3l8->s15, t3l8->s16,
        t3l8->s21, t3l8->s22, t3l8->s23, t3l8->s24, t3l8->s25, t3l8->s26, t3l8->s31, t3l8->s32 };
    foreach(QCheckBox * item, itemList8) {
        channelDistinguish += get_checkedText(item);
    }
    XReport::Get()->itemMap["channelDistinguish"] = channelDistinguish;
    //体质辨识
    QString physiqueDistinguish;
    QList<QCheckBox *> itemList9 = { t3l9->s11, t3l9->s12, t3l9->s13, t3l8->s14, t3l8->s15, t3l9->s16,
        t3l9->s21, t3l9->s22, t3l9->s23 };
    foreach(QCheckBox * item, itemList9) {
        physiqueDistinguish += get_checkedText(item);
    }
    XReport::Get()->itemMap["physiqueDistinguish"] = physiqueDistinguish;
    //重大疾病预警
    QString importDiseaseWarning;
    QList<QCheckBox *> itemList10 = { t3l10->s11, t3l10->s12, t3l10->s13, t3l8->s14, t3l8->s15, t3l10->s16 };
    foreach(QCheckBox * item, itemList10) {
        importDiseaseWarning += get_checkedText(item);
    }
    XReport::Get()->itemMap["importDiseaseWarning"] = importDiseaseWarning;
    //亚健康肝
    QString subHealthyLiver;
    QList<QCheckBox *> itemList11_1 = { t3l11->s12, t3l11->s13, t3l11->s14, t3l11->s15, t3l11->s16 };
    foreach(QCheckBox * item, itemList11_1) {
        subHealthyLiver+= get_checkedText(item);
    }
    XReport::Get()->itemMap["subHealthyLiver"] = subHealthyLiver;
    //亚健康心
    QString subHealthyHeart;
    QList<QCheckBox *> itemList11_2 = { t3l11->s22, t3l11->s23, t3l11->s24, t3l11->s25, t3l11->s26 };
    foreach(QCheckBox * item, itemList11_2) {
        subHealthyHeart+= get_checkedText(item);
    }
    XReport::Get()->itemMap["subHealthyHeart"] = subHealthyHeart;
    //亚健康脾
    QString subHealthySpleen;
    QList<QCheckBox *> itemList11_3 = { t3l11->s32, t3l11->s33, t3l11->s34, t3l11->s35, t3l11->s36 };
    foreach(QCheckBox * item, itemList11_3) {
        subHealthySpleen+= get_checkedText(item);
    }
    XReport::Get()->itemMap["subHealthySpleen"] = subHealthySpleen;
    //亚健康肺
    QString subHealthyLung;
    QList<QCheckBox *> itemList11_4 = { t3l11->s42, t3l11->s43, t3l11->s44, t3l11->s45, t3l11->s46 };
    foreach(QCheckBox * item, itemList11_4) {
        subHealthyLung+= get_checkedText(item);
    }
    XReport::Get()->itemMap["subHealthyLung"] = subHealthyLung;
    //亚健康肾
    QString subHealthyKidney;
    QList<QCheckBox *> itemList11_5 = { t3l11->s52, t3l11->s53, t3l11->s54 };
    foreach(QCheckBox * item, itemList11_5) {
        subHealthyKidney+= get_checkedText(item);
    }
    XReport::Get()->itemMap["subHealthyKidney"] = subHealthyKidney;
    //现有疾病预警
    QString existDiseaseWarning;
    QList<QCheckBox *> itemList12 = { t3l12->s11, t3l12->s12, t3l12->s13,
        t3l12->s31, t3l12->s32, t3l12->s33, t3l12->s31, t3l12->s32, t3l12->s33,
        t3l12->s41, t3l12->s42, t3l12->s43, t3l12->s51, t3l12->s52 };
    foreach(QCheckBox * item, itemList12) {
        existDiseaseWarning += get_checkedText(item);
    }
    XReport::Get()->itemMap["existDiseaseWarning"] = existDiseaseWarning;

    QMap<QString, QString> itemMap = XReport::Get()->itemMap;
    QMap<QString, QString>::iterator item;
    for (item = itemMap.begin(); item != itemMap.end(); item++) {
        qDebug() << item.key() << "\t" << item.value();
    }

    //assessReportPage->reportView->page()->setHtml(XReport::Get()->create_SCReport());
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.16
    Description	: 使用系统打印机打印报告 & 槽函数
 *****************************************************************************/
void Summer::print_report(void)
{
    qDebug() << "current is " << current;
    if (current == assessReportWidget) {
        QPrintDialog dlg(XReport::Get()->printer, this);
        if (dlg.exec() == QDialog::Accepted) {
            assessReportPage->reportView->page()->print(XReport::Get()->printer, [this](bool found){
                //if (!found) QMessageBox::information(ui->textBrowser, QString(), QStringLiteral("找不到打印机"));
            });
            //mark fixme 打印不跳转
            //create_recuperatePlanPage();
        }
    }
}

void Summer::create_baseInfoPage()
{
    ui->label->show();
    ui->label_2->show();
    ui->commit->show();

    disconnect(ui->commit, 0, 0, 0);
    connect(ui->commit, SIGNAL(clicked()), this, SLOT(commit_baseInfo()));

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
    baseInfoWidget->move(0, 73);
    baseInfoPage = new Ui::baseInfo;
    baseInfoPage->setupUi(baseInfoWidget);

    baseInfoWidget->show();
    current = baseInfoWidget;
}


/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.11
    Description	: 创建tabListWidget控件
 *****************************************************************************/
static int contentHeight = 0;
template <typename T>
T create_tlw(QListWidget * parent, T ui)
{
    QWidget *tlw = new QWidget(parent);

    ui->setupUi(tlw);
    int index = parent->count();
    new QListWidgetItem(parent);
    parent->setItemWidget(parent->item(index), ui->gridLayoutWidget);
    parent->item(index)->setSizeHint(tlw->size());
    contentHeight += tlw->height();
    tlw->resize(0, 0);

    return ui;
}

int testvalue = 0;

//使用事件过滤器在父控件处理滚轮事件
bool Summer::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        if (current == selfCheckWidget) {
            selfCheckPage->verticalScrollBar->setValue(tabListWidget->verticalScrollBar()->value());
        }
        return false;        //因为要同步原滚动条,不能过滤掉,继续传送给子控件
    }

    return false;
}

void Summer::create_selfCheckPage()
{
    disconnect(ui->commit, 0, 0, 0);
    connect(ui->commit, SIGNAL(clicked()), this, SLOT(commit_selfCheck()));

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
    selfCheckWidget->move(0, 73);
    selfCheckPage = new Ui::selfCheck;
    selfCheckPage->setupUi(selfCheckWidget);

    tabListWidget = new QListWidget(selfCheckPage->widget);
    tabListWidget->setGeometry(QRect(28, 28, 1090, 680));
    tabListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabListWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    tabListWidget->setStyleSheet("padding:4px;");

    tabListWidget->verticalScrollBar()->setSingleStep(20);
    tabListWidget->verticalScrollBar()->installEventFilter(this);

    //若需要泛型可用typeid获得类型id,dynamic_cast强制转化为制定对象类型
    t3l0 = create_tlw(tabListWidget, new Ui::T3L0);
    t3l1 = create_tlw(tabListWidget, new Ui::T3L1);
    t3l2 = create_tlw(tabListWidget, new Ui::T3L2);
    t3l3 = create_tlw(tabListWidget, new Ui::T3L3);
    t3l4 = create_tlw(tabListWidget, new Ui::T3L4);
    t3l5 = create_tlw(tabListWidget, new Ui::T3L5);
    t3l6 = create_tlw(tabListWidget, new Ui::T3L6);
    //休息时间、吸烟、喝酒、饮茶饮咖啡、饮水、运动单选
    t3l7 = create_tlw(tabListWidget, new Ui::T3L7);
    QButtonGroup* t3l7g1 = new QButtonGroup(selfCheckWidget);
    t3l7g1->addButton(t3l7->s12, 2);
    t3l7g1->addButton(t3l7->s13, 3);
    t3l7g1->addButton(t3l7->s14, 4);
    t3l7g1->addButton(t3l7->s15, 5);
    QButtonGroup* t3l7g2 = new QButtonGroup(selfCheckWidget);
    t3l7g2->addButton(t3l7->s22, 2);
    t3l7g2->addButton(t3l7->s23, 3);
    t3l7g2->addButton(t3l7->s24, 4);
    QButtonGroup* t3l7g3 = new QButtonGroup(selfCheckWidget);
    t3l7g3->addButton(t3l7->s32, 2);
    t3l7g3->addButton(t3l7->s33, 3);
    t3l7g3->addButton(t3l7->s34, 4);
    QButtonGroup* t3l7g4 = new QButtonGroup(selfCheckWidget);
    t3l7g4->addButton(t3l7->s42, 2);
    t3l7g4->addButton(t3l7->s43, 3);
    t3l7g4->addButton(t3l7->s44, 4);
    QButtonGroup* t3l7g5 = new QButtonGroup(selfCheckWidget);
    t3l7g5->addButton(t3l7->s52, 2);
    t3l7g5->addButton(t3l7->s53, 3);
    QButtonGroup* t3l7g8 = new QButtonGroup(selfCheckWidget);
    t3l7g8->addButton(t3l7->s82, 2);
    t3l7g8->addButton(t3l7->s83, 3);
    t3l7g8->addButton(t3l7->s84, 4);
    QButtonGroup* t3l7g9 = new QButtonGroup(selfCheckWidget);
    t3l7g9->addButton(t3l7->s92, 2);
    t3l7g9->addButton(t3l7->s93, 3);
    t3l7g9->addButton(t3l7->s94, 4);
    t3l8 = create_tlw(tabListWidget, new Ui::T3L8);
    t3l9 = create_tlw(tabListWidget, new Ui::T3L9);
    t3l10 = create_tlw(tabListWidget, new Ui::T3L10);
    t3l11 = create_tlw(tabListWidget, new Ui::T3L11);
    t3l12 = create_tlw(tabListWidget, new Ui::T3L12);

    //这个计算是 滚动内容高度 - 已经显示高度, 单位是像素值 setVerticalScrollMode(QListWidget::ScrollPerPixel)
    selfCheckPage->verticalScrollBar->setMaximum(contentHeight-tabListWidget->height());
#if 0
    //实现自定义的滚动条,但无法分离滚动条和显示区
    //tabListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabListWidget->setVerticalScrollBar(selfCheckPage->verticalScrollBar);
#endif
    connect(selfCheckPage->verticalScrollBar, SIGNAL(valueChanged(int)), tabListWidget->verticalScrollBar(), SLOT(setValue(int)));
    //查表实现？
    //connect(tabListWidget->pos, SIGNAL(currentRowChanged(int)), selfCheckPage->verticalScrollBar, SLOT(setValue(int)));
    selfCheckWidget->show();

    current = selfCheckWidget;
}

#define YaQian_HSV  11
void Summer::create_irExamPage()
{
    disconnect(ui->commit, 0, 0, 0);
    connect(ui->commit, SIGNAL(clicked()), this, SLOT(commit_irExam()));

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
    irExamWidget->move(0, 73);
    irExamPage = new Ui::irExam;
    irExamPage->setupUi(irExamWidget);
    irExamWidget->show();
    //irDev signal & slot
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(irExamPage->irRefresh, SIGNAL(clicked()), this, SLOT(show_devList()));
    connect(irExamPage->irConnect, SIGNAL(clicked()), this, SLOT(connect_irDev()));
    connect(irExamPage->irDisconnect, SIGNAL(clicked()), this, SLOT(disconnect_irDev()));
    connect(irExamPage->Play, SIGNAL(clicked()), XDev::Get(), SLOT(play_irDev()));
    connect(XDev::Get(), SIGNAL(magFrame(cv::Mat)), irExamPage->View, SLOT(play(cv::Mat)));
    connect(irExamPage->Pause, SIGNAL(clicked()), XDev::Get(), SLOT(stop_irDev()));
    connect(irExamPage->Photo, SIGNAL(clicked()), irExamPage->View, SLOT(photoBody()));
    connect(irExamPage->Photo_2, SIGNAL(clicked()), irExamPage->View, SLOT(photoBreast()));
    connect(irExamPage->AutoFocus, SIGNAL(clicked()), XDev::Get(), SLOT(auto_focus()));
    connect(irExamPage->FarFocus, SIGNAL(clicked()), XDev::Get(), SLOT(far_focus()));
    connect(irExamPage->NearFocus, SIGNAL(clicked()), XDev::Get(), SLOT(near_focus()));
    //image list in widget del & show
    connect(irExamPage->Del, SIGNAL(clicked()), irExamPage->ImageList, SLOT(del_image()));
    connect(irExamPage->Del_2, SIGNAL(clicked()), irExamPage->ImageList_2, SLOT(del_image()));
    irExamPage->ImageList->setup_dir("c:/rrk/picture/body/");
    connect(irExamPage->Show, SIGNAL(clicked()), irExamPage->ImageList, SLOT(show_image()));
    irExamPage->ImageList_2->setup_dir("c:/rrk/picture/breast/");
    connect(irExamPage->Show_2, SIGNAL(clicked()), irExamPage->ImageList_2, SLOT(show_image()));
    connect(irExamPage->ImageList, SIGNAL(show_image(QString)), irExamPage->View, SLOT(show_image(QString)));
    connect(irExamPage->ImageList_2, SIGNAL(show_image(QString)), irExamPage->View, SLOT(show_image(QString)));
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
    disconnect(ui->commit, 0, 0, 0);
    connect(ui->commit, SIGNAL(clicked()), this, SLOT(print_report()));

    if (assessReportWidget != NULL) {
        if (current != assessReportWidget) {
            current->hide();
            assessReportWidget->show();
            current =  assessReportWidget;
        }
        assessReportPage->reportView->page()->setHtml(XReport::Get()->get_html());
        return;
    }

    if (current != NULL)
        current->hide();

    assessReportWidget = new QWidget(this);
    assessReportWidget->move(0, 73);
    assessReportPage = new Ui::assessReport;
    assessReportPage->setupUi(assessReportWidget);

    assessReportPage->reportView->page()->setHtml(XReport::Get()->get_html());
    assessReportWidget->show();

    //TODO
    //connect(assessReportPage->printB, SIGNAL(clicked()), this, SLOT(print_report()));

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
    recuperatePlanWidget->move(0, 73);
    recuperatePlanPage = new Ui::recuperatePlan;
    recuperatePlanPage->setupUi(recuperatePlanWidget);
    recuperatePlanWidget->show();
    current = recuperatePlanWidget;
}

//=============================================================================
//=============================================================================
//=============================================================================
