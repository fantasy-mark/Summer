#include "xview.h"
#include "xpro.h"
#include <vector>
#include "kitconfig.h"
#include "xsec.h"
#include "logsys.h"
#include <QPainter>
#include <QPixmap>

#include "logsys.h"
#include <malloc.h>

#include "xdev.h"
#include "xconfig.h"

using namespace std;

static VideoCapture irCapture;

/******************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.04.28
	Description	: 打开摄像头 \ 创建处理视频帧的线程
 ******************************************************************************/
XView::XView(QWidget *parent)
	: QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 虚构函数
 *****************************************************************************/
XView::~XView()
{
}

/******************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.04.28
	Description	: 绘图到IrView
 ******************************************************************************/
void XView::paintEvent(QPaintEvent * event)
{
	pen.begin(this);
    pen.drawImage(QPoint(0, 0), img);

    //点温度显示
    if (m_currentPoint.x() > 0 && m_currentPoint.y() > 0) {
        int r = XConfig::Get()->value("expandRatio").toInt();
        int temperature = XDev::Get()->GetTemperature(m_currentPoint.x() / r, m_currentPoint.y() / r);
        pen.drawText(m_currentPoint, QString::number(temperature / 1000.0, 'f', 1));
    }

	pen.end();
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 鼠标移动事件,必须 setMouseTracking(true)
 *****************************************************************************/
void XView::mouseMoveEvent(QMouseEvent *event)
{
	m_currentPoint = event->pos();
	repaint();
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 鼠标离开事件
 *****************************************************************************/
void XView::leaveEvent(QEvent * event)
{
    m_currentPoint.setX(-1);
    m_currentPoint.setY(-1);
    repaint();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.07
	Description	: 显示所选图像 & 槽函数
 *****************************************************************************/
void XView::show_image(QString path)
{
    if (MAG_IsProcessingImage(0))
        MAG_StopProcessImage(0);

    QString ddt = path.replace(".png", ".ddt");
    if (!XDev::Get()->loadDDT(ddt)) {
        qDebug("Load ddt failed");
    }
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 用于显示照片和巨哥温度流
 *****************************************************************************/
void XView::play(cv::Mat mat)
{
    switch (mat.channels()) {
	case 1:
        //注意此处是转化为线性rgb的3位宽数据
        cvtColor(mat, mat, CV_GRAY2RGB, 0);
		break;
	case 3:
		break;
	default:
		PrWarning("不支持的Mat格式");
		return;
    }

    rotate(mat, mat, ROTATE_180);

    int w = XConfig::Get()->value("videoW").toInt();
    int h = XConfig::Get()->value("videoH").toInt();

    cv::resize(mat, mat, Size(w, h));

    if (img.isNull()) {
        uchar *buf = new uchar[mat.rows * mat.cols * 3];
        img = QImage(buf, mat.cols, mat.rows, QImage::Format_RGB888);
	}
	if (cm.isNull()) {
		uchar *cmbuf = new uchar[cmMat.rows * cmMat.cols * cmMat.elemSize()];
		cm = QImage(cmbuf, cmMat.cols, cmMat.rows, QImage::Format_RGB888);
		set_colormap(11);
	}

	//伪彩映射
    LUT(mat, cmTable, view);
    memcpy(img.bits(), view.data, view.rows * view.cols * view.elemSize());

	update();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.06
    Description	: 全身拍照 & 槽函数
 *****************************************************************************/
void XView::photoBody()
{
	QDateTime currentTime = QDateTime::currentDateTime();
    QString current = PICTURE_BODY_PATH + currentTime.toString("yyyy_MM_dd_hh_mm_ss");
    QString current_pic = current + ".png";
    QString current_data = current + ".ddt";
    Mat saveMat;
    //open以bgr排列储存数据,故需要转化后保存
    cvtColor(view, saveMat, CV_BGR2RGB);
	//因需求改变,直接保存彩色图
    imwrite(current_pic.toStdString(), saveMat);
    XDev::Get()->saveDDT(current_data);
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.06.06
    Description	: 乳腺拍照 & 槽函数
 *****************************************************************************/
void XView::photoBreast()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString current = PICTURE_BREAST_PATH + currentTime.toString("yyyy_MM_dd_hh_mm_ss");
    QString current_pic = current + ".png";
    QString current_data = current + ".ddt";
    Mat saveMat;
    //open以bgr排列储存数据,故需要转化后保存
    cvtColor(view, saveMat, CV_BGR2RGB);
    //因需求改变,直接保存彩色图
    imwrite(current_pic.toStdString(), saveMat);
    XDev::Get()->saveDDT(current_data);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.12
	Description	: 设置色卡 & 槽函数
 *****************************************************************************/
bool XView::set_colormap(int index)
{
	UCHAR const * pBarData;
	BITMAPINFO const * pBarInfo;

	Mat Nice;

	if (index > 10) {
		switch (index) {
		case 11:
			XPro::Get()->rainbowColorMap();
			rotate(XPro::Get()->get_colormap(), cmTable, ROTATE_90_COUNTERCLOCKWISE);
			cv::resize(cmTable, cmMat, cmMat.size());
            rotate(cmMat, cmMat, ROTATE_180);
            return true;
		case 12:
            Nice = imread("C:/Users/Administrator/source/repos/Summer/doc/Nice.bmp", CV_LOAD_IMAGE_UNCHANGED);
            if (Nice.empty())
                return false;
			for (int i = 0; i < 256; i++) {
				cmTable.at<Vec3b>(255 - i, 0)[0] = Nice.at<Vec3b>(i, 0)[2];
				cmTable.at<Vec3b>(255 - i, 0)[1] = Nice.at<Vec3b>(i, 0)[1];
				cmTable.at<Vec3b>(255 - i, 0)[2] = Nice.at<Vec3b>(i, 0)[0];
				cv::resize(cmTable, cmMat, cmMat.size());
			}
            return true;
		default:
            break;
        }
    } else {
        XDev::Get()->SetColorPalette((ColorPalette)index);
		//pData-指向颜色条位图数据区指针, pInfo-指向颜色条位图信息区指针
		//TODO 此处有坑，注意先要初始化巨哥摄像头后才可以获取成功读取相关数据
        if (XDev::Get()->GetOutputColorBardata(&pBarData, &pBarInfo)) {
			//此处是调色板Mat TODO
			for (int i = 0; i < 256; i++) {
				int b0 = (int)*((uchar *)(pBarInfo->bmiColors) + 4 * i);
				int b1 = (int)*((uchar *)(pBarInfo->bmiColors) + 4 * i + 1);
				int b2 = (int)*((uchar *)(pBarInfo->bmiColors) + 4 * i + 2);
				cmTable.at<Vec3b>(255 - i, 0) = Vec3b(b2, b1, b0);
				cv::resize(cmTable, cmMat, cmMat.size());
			}
            imshow("show table", cmMat);
            return true;
        } else {
            PrWarning("获取失败，巨哥摄像头是否已经初始化成功(连接成功?)");
        }
    }

    return false;
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.13
    Description	: 获得色卡映射Mat
 *****************************************************************************/
cv::Mat XView::get_cmTable()
{
    return cmTable;
}
