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
	void add_image(const QString &path);		// 目录变化调用，path是监控路径
	void del_image();
	void show_image();

signals:
	void show_image(QString);

private:
	QStringList currentFiles;	// 当前每个监控的内容目录列表

protected:
	QFileSystemWatcher myWatcher;
};
