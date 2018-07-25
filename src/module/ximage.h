#pragma once

#include <QDir>
#include <QListWidget>
#include <QFileSystemWatcher>

#include "xview.h"

class XImage : public QListWidget
{
	Q_OBJECT

public:
	XImage(QWidget *parent);
	~XImage();

public slots:
	void add_image(const QString &path);		// Ŀ¼�仯���ã�path�Ǽ��·��
	void del_image();
	void show_image();

signals:
	void show_image(QString);

private:
	QStringList currentFiles;	// ��ǰÿ����ص�����Ŀ¼�б�

protected:
	QFileSystemWatcher myWatcher;
};
