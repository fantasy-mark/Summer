#include "ximage.h"
#include <QDateTime>
#include <qDebug>

QString basePath = "C:/rrk/picture/";//QDir::currentPath() + "/Resources/picture/";

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: 设置图像栏
 *****************************************************************************/
XImage::XImage(QWidget *parent)
	: QListWidget(parent)
{
}

XImage::~XImage()
{
}

void XImage::setup_dir(QString dirPath)
{
    path = dirPath;
    myWatcher.addPath(path);

    const QDir dir(path);
    currentFiles = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    connect(&myWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(add_image(QString)));

    //设置图片列表框属性
    //设置QListWidget的显示模式 \ 图片大小 \ 间距
    this->setViewMode(QListView::IconMode);
    this->setIconSize(QSize(160, 160));
    this->setSpacing(16);
    //设置自动适应布局调整（Adjust适应，Fixed不适应）\ 不能移动
    this->setResizeMode(QListWidget::Adjust);
    this->setMovement(QListWidget::Static);
    //设置选择模式 单选:SingleSelection 点击+ctrl多选:MultiSelection
    //点击和鼠标复合多选:ExtendedSelection 鼠标拖拉多选:ContiguousSelection
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    for (int i = 0; i < currentFiles.size(); i++) {
        if (currentFiles[i].isNull()) continue;
        if (currentFiles[i].contains(".ddt", Qt::CaseInsensitive)) continue;
        QListWidgetItem *new_item = new QListWidgetItem;
        new_item->setSizeHint(QSize(90, 120));
        new_item->setIcon(QIcon(path + currentFiles[i]));

        //设置toolTip是为了保存文件路径,并且隐藏
        new_item->setToolTip(path + currentFiles[i]);

        //把新加入图像放到最前
        this->addItem(new_item);
    }
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: 用于实时监视目录文件 & 槽函数
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

	//防止未写完成, 导致读失败
	QDateTime last = QDateTime::currentDateTime();
	QDateTime now;
	for (;;) {
		now = QDateTime::currentDateTime();
		if (last.msecsTo(now) >= 15)	{
			break;
		}
	}

	foreach(QString file, newFile) {
        if (file.contains(".ddt", Qt::CaseInsensitive)) continue;
        QListWidgetItem *new_item = new QListWidgetItem;
		new_item->setSizeHint(QSize(90, 120));
		new_item->setIcon(QIcon(path + file));

        //设置toolTip是为了保存文件路径,并且隐藏
        new_item->setToolTip(path + file);

		this->addItem(new_item);
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: 删除所选图像 & 槽函数
 *****************************************************************************/
void XImage::del_image()
{
	QList<QListWidgetItem*> itemList = this->selectedItems();
    for (int i = 0; i < itemList.size(); i++) {
        QFile file(itemList[i]->toolTip());
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
	Description	: 双击图片显示 & 槽函数
 *****************************************************************************/
void XImage::show_image()
{
	QListWidgetItem * item = this->currentItem();
    if (item == NULL)
        return;
    QString file_path = item->toolTip();

    emit show_image(file_path);
}
