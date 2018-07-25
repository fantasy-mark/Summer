#include "ximage.h"
#include <QDateTime>
#include <qDebug>

QString path = "C:/rrk/picture/";//QDir::currentPath() + "/Resources/picture/";

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: ����ͼ����
 *****************************************************************************/
XImage::XImage(QWidget *parent)
	: QListWidget(parent)
{
	myWatcher.addPath(path);

	const QDir dir(path);
	currentFiles = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	connect(&myWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(add_image(QString)));

	//����ͼƬ�б������
	//����QListWidget����ʾģʽ \ ͼƬ��С \ ���
	this->setViewMode(QListView::IconMode);
	this->setIconSize(QSize(160, 160));
	this->setSpacing(16);
	//�����Զ���Ӧ���ֵ�����Adjust��Ӧ��Fixed����Ӧ��\ �����ƶ�
	this->setResizeMode(QListWidget::Adjust);
	this->setMovement(QListWidget::Static);
	//����ѡ��ģʽ ��ѡ:SingleSelection ���+ctrl��ѡ:MultiSelection
	//�������긴�϶�ѡ:ExtendedSelection ���������ѡ:ContiguousSelection 
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);

	for (int i = 0; i < currentFiles.size(); i++) {
		if (currentFiles[i].isNull()) continue;
		QListWidgetItem *new_item = new QListWidgetItem;
		new_item->setSizeHint(QSize(90, 120));
		new_item->setIcon(QIcon(path + currentFiles[i]));

		//����text��Ϊ�˱����ļ�·��,��������
		new_item->setHidden(true);
		new_item->setText(path + currentFiles[i]);

		//���¼���ͼ��ŵ���ǰ
		this->addItem(new_item);
	}
}

XImage::~XImage()
{
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: ����ʵʱ����Ŀ¼�ļ� & �ۺ���
 *****************************************************************************/
void XImage::add_image(const QString &path)
{
	const QDir dir(path);
	QStringList newFlieList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
	QSet<QString> newDirSet = QSet<QString>::fromList(newFlieList);
	QSet<QString> currentDirSet = QSet<QString>::fromList(currentFiles);
	QSet<QString> newFiles = newDirSet - currentDirSet;
	currentFiles = newFlieList;
	QStringList newFile = newFiles.toList();

	//��ֹδд���, ���¶�ʧ��
	QDateTime last = QDateTime::currentDateTime();
	QDateTime now;
	for (;;) {
		now = QDateTime::currentDateTime();
		if (last.msecsTo(now) >= 15)	{
			break;
		}
	}

	foreach(QString file, newFile) {
		QListWidgetItem *new_item = new QListWidgetItem;
		new_item->setSizeHint(QSize(90, 120));
		new_item->setIcon(QIcon(path + file));

		//����text��Ϊ�˱����ļ�·��,��������
		new_item->setHidden(true);
		new_item->setText(path + file); 

		this->addItem(new_item);
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: ɾ����ѡͼ�� & �ۺ���
 *****************************************************************************/
void XImage::del_image()
{
	QList<QListWidgetItem*> itemList = this->selectedItems();
	for (int i = 0; i < itemList.size(); i++) {
		QFile file(itemList[i]->text());
		if (file.exists()) {
			file.remove();
		}

		delete itemList[i];

		const QDir dir(path);
		currentFiles = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.08
	Description	: ˫��ͼƬ��ʾ & �ۺ���
 *****************************************************************************/
void XImage::show_image()
{
	QListWidgetItem * item = this->currentItem();
	QString file_path = item->text();
	emit show_image(file_path);
}
