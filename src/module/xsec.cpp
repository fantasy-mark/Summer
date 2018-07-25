#include "xsec.h"

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.16
	Description	: ͨ���޸��ļ�ͷʵ�ּӽ���Ӱ���ļ�
 *****************************************************************************/
XSec::XSec()
{
}

XSec::~XSec()
{
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.28
	Description	: ������Ƭ
 *****************************************************************************/
void XSec::en_sec(QString path)
{
	//���ڼ����ļ�
	QFile file(path);
	if (!file.open(QFile::ReadWrite)) {
		qDebug() << "Can't open file for writing";
	}
	QDataStream in(&file);
	QString msg = "yaqian";
	for (int i = 0; i < msg.length(); i++) {
		in << (unsigned char)msg.at(i).toLatin1();
	}
	file.close();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.28
	Description	: ������Ƭ
 *****************************************************************************/
void XSec::de_sec(QString path)
{
	//���ڽ����ļ�
	QFile file(path);
	if (!file.open(QFile::ReadWrite)) {
		qDebug() << "Can't open file for writing";
	}
	QDataStream in(&file);
	char magic[6] = { char(0x89), 'P', 'N', 'G', 0x0d, 0x0a };
	QString msg = QString(QLatin1String(magic));
	for (int i = 0; i < msg.length(); i++) {
		in << (unsigned char)msg.at(i).toLatin1();
	}
	file.close();
}
