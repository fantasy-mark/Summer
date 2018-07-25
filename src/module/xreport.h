#pragma once

#include <QObject>
#include <QPrinter>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.26
	Description	: 报告的tag项和方法
 *****************************************************************************/
class XReport : public QObject
{
	Q_OBJECT

public:
	static XReport * Get();
	~XReport();
	QPrinter * printer;
	QNetworkAccessManager * netManager;
	QString html;
	QMap<QString, QString> itemMap;

	template <typename T>
	QString get_checkedText(T item);

    QString create_Report();
	QString create_BIReport();
	QString create_SCReport();

public slots:
	//服务器API
	void reply_Get_Finished(QNetworkReply * reply);
	void reply_Post_Finished(QNetworkReply * reply);
	void test_get();
	void test_post();
	
protected:
	XReport();
};
