#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <qDebug>

#include "xreport.h"
#include "logsys.h"

XReport::XReport()
{
	//���ô�ӡ��
	printer = new QPrinter;

	//printer->setOutputFormat(QPrinter::PdfFormat);
	printer->setPageSize(QPrinter::A4);
	//printer->setOutputFileName("hello.pdf");

	netManager = new QNetworkAccessManager(this);
	connect(netManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(reply_Get_Finished(QNetworkReply *)));
}

XReport::~XReport()
{
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.26
	Description	: ��þ�̬��XReportʵ��ָ��
 *****************************************************************************/
XReport * XReport::Get()
{
	static XReport xr;
	return &xr;
}

void XReport::reply_Get_Finished(QNetworkReply * reply)
{
    QByteArray data = reply->readAll();

	QJsonParseError *error = new QJsonParseError;
	QJsonDocument doc = QJsonDocument::fromJson(data, error);
	if (error->error == QJsonParseError::NoError) {
		if (doc.isObject()) {
			QByteArray a = doc.toJson();
			qDebug() << QString(a);
		}
		// !!! ��Ϊ����[]��isArray�ж�, ����{}��isObject�ж�
		// ����Ƕ�׶�Ӧ /data/inspectionInfo/api/name/index/ name��index
		if (doc.isArray() && doc.array().at(0).isObject())	{
			QJsonObject base = doc.array().at(0).toObject();
			QJsonObject client = base.find("client").value().toObject();

			//ui->lineEdit->setText(client.find("name").value().toString());
			//ui->lineEdit_2->setText(client.find("sex").value().toString());
			//ui->lineEdit_3->setText(QString::number(client.find("age").value().toDouble()));
			//ui->lineEdit_4->setText(client.find("phone").value().toString());
			if (client.find("name").value().isString())
				Pr("name is string");
				qDebug() << client.find("name").value().toString();;
			if (client.find("age").value().isDouble())
				Pr("age is double");
		} else {
			if (doc.isArray())
				Pr("doc is array");
			qDebug() << "json data format error";
		}
	} else {
		qDebug() << "error" << error->errorString();
	}
		
    reply->deleteLater();
}

void XReport::reply_Post_Finished(QNetworkReply* reply)
{
//	QByteArray data = reply->readAll();
//	qDebug() << QString(data);
//	reply->deleteLater();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.10
	Description	: ����GET http://192.168.124.222/data/inspectionInfo/api & �ۺ���
 *****************************************************************************/
void XReport::test_get()
{
	QUrl url("http://192.168.124.222/data/inspectionInfo/api");		// ��������url
	QNetworkRequest request = QNetworkRequest(url);					// �༭HTTPͷ��

	//û������Authorization header, QtԴ����ʱδ�ҵ���Դ����
	//url.setUserName("api-client");									// api��¼��֤
	//url.setPassword("123-uijk");
	//url.setPort(80);
	//"Basic " + UserName:Passwordʹ��base64���ɵ��ַ���
	request.setRawHeader("Authorization", "Basic YXBpLWNsaWVudDoxMjMtdWlqaw==");

	//connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyPostFinished(QNetworkReply*)));

	qDebug() << request.rawHeaderList();
	qDebug() << request.url().scheme();
	qDebug() << request.url().authority();
	qDebug() << request.url().port();
	qDebug() << request.rawHeaderList();

	netManager->get(request);
}

///*****************************************************************************
//Copyright	: Yaqian Group
//Author		: Mark_Huang ( hacker.do@163.com )
//Date		: 2018.05.11
//Description	: ����POST http://192.168.124.222/data/inspectionInfo/api & �ۺ���
//*****************************************************************************/
void XReport::test_post()
{
//	QUrl url("http://192.168.124.222/data/inspectionInfo/client/1/api");		// ��������url
//	url.setUserName("api-client");									// api��¼��֤
//	url.setPassword("123-uijk");
//	QNetworkRequest request = QNetworkRequest(url);					// �༭HTTPͷ��
//
//	netManager = new QNetworkAccessManager(this);
//
//	// Json����, ����������ʹ��QString::fromLocal8Bit("XX")
//	QJsonObject json;
//	json.insert("name", ui->lineEdit->text());
//	json.insert("sex", ui->lineEdit_2->text());
//	json.insert("age", ui->lineEdit_3->text());
//	json.insert("phone", ui->lineEdit_4->text());
//
//	QJsonDocument document;
//	document.setObject(json);
//	QByteArray dataArray = document.toJson(QJsonDocument::Compact);
//
//	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//	connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyPostFinished(QNetworkReply*)));
//
//	netManager->put(request, dataArray);
}

QString XReport ::create_Report(void)
{
    QByteArray text;
    QFile file("C:/Users/Administrator/source/repos/Summer/doc/temple.html");
    if (file.open(QIODevice::ReadOnly)) {		//��ֻ����ʽ��
        text = file.readAll();
        file.close();
    }

    html = QString(text);

    return html;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.13
	Description	: ���ɻ�����Ϣ����
 *****************************************************************************/
QString XReport::create_BIReport(void)
{
	QStringList baseInfoItemList = { "name", "sex", "age", "tel", "id", "height", "weight", "bmi", 
		"temp", "fatR", "subFatR", "viscusFatR", "metabolicR", "boneMass", "bodyMoisture", "bodyAge", "bloodSugar",
		"wristPulseL", "wristPulseLH", "wristPulseLL", "wristPulseR", "wristPulseRH", "wristPulseRL",
		"armPulseL", "armPulseLH", "armPulseLL", "armPulseR", "armPulseRH", "armPulseRL" };
	foreach(QString item, baseInfoItemList) {
		QRegExp rx("%" + item + "%");
		html.replace(rx, XReport::Get()->itemMap[item]);
	}

	return html;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.13
	Description	: �����Բ���Ϣ����
 *****************************************************************************/
QString XReport::create_SCReport(void)
{
	QStringList selfCheckItemList = { "familyHistory", "allergicHistory", "pastHistory",
		"drugHistory", "hurtHistory", "surgeryHistory", "breastSymptom", "lifeHabit",
		"channelDistinguish", "physiqueDistinguish", "breastSymptom", "importDiseaseWarning"
		"existDiseaseWarning", "subHealthyLiver", "subHealthyHeart", "subHealthySpleen",
		"subHealthyLung", "subHealthyKidney" };
	foreach(QString item, selfCheckItemList) {
		QRegExp rx("%" + item + "%");
		html.replace(rx, XReport::Get()->itemMap[item]);
	}

	return html;
}
