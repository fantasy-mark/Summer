#pragma once
#include <qDebug>
#include <QFile>
#include <QDataStream>

class XSec
{
public:
	XSec();
	~XSec();
	static void en_sec(QString path);
	static void de_sec(QString path);
};
