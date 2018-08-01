#pragma once

#include <QObject>
#include <QPrinter>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.26
	Description	: �����tag��ͷ���
 *****************************************************************************/
class XReport : public QObject
{
	Q_OBJECT

public:
	static XReport * Get();
	~XReport();
	QPrinter * printer;
	QNetworkAccessManager * netManager;
	QMap<QString, QString> itemMap;

	template <typename T>
	QString get_checkedText(T item);

    void create_Report();

    QString get_html();

public slots:
	//������API
	void reply_Get_Finished(QNetworkReply * reply);
	void reply_Post_Finished(QNetworkReply * reply);
	void test_get();
	void test_post();
    //���ɱ���
    QString create_BIReport();
    QString create_SCReport();

private:
    QString html;

protected:
	XReport();
};
