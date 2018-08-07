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
	//setMouseTracking(true);

	//Temp = new QLabel(parent);
	//Temp->setObjectName(QStringLiteral("Temp"));
	//Temp->setGeometry(QRect(1020, 810, 351, 61));
	//Temp->setStyleSheet(QStringLiteral("background-color: rgb(129, 112, 255);font: 75 12pt \"Aharoni\";"));
	//Temp->raise();
	//Temp->show();
}

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
#if 0
	if (m_currentPoint.x() > 124) {
		//Mat旋转了180度,温度坐标相应调整
		int temperature = XDev::Get()->get_mgdevice()->GetTemperatureProbe(
			288-(m_currentPoint.x() - 124), 384-(m_currentPoint.y() - 208), 5);
		pen.drawText(m_currentPoint, QString::number(temperature / 1000.0, 'f', 1));
	}
#endif
#if 0
	if (!cm.isNull()) {
		pen.drawImage(QPoint(536, 0), cm);
	}
#endif
	pen.end();
}

void XView::mouseMoveEvent(QMouseEvent *event)
{
	m_currentPoint = event->pos();
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
    //XSec::de_sec(path);
	Mat mat = imread(path.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
    //XSec::en_sec(path);

	switch (mat.channels()) {
	case 1:
        cvtColor(mat, mat, CV_GRAY2RGB, 0);
		break;
	case 3:
		break;
	default:
		PrWarning("不支持的Mat格式");
		break;
	}

    cvtColor(mat, view, CV_BGR2RGB);
    if (img.isNull()) {
		uchar *buf = new uchar[view.rows * view.cols * view.elemSize()];
		img = QImage(buf, view.cols, view.rows, QImage::Format_RGB888);
	}

    XDev::Get()->stop_irDev();

	memcpy(img.bits(), view.data, view.rows * view.cols * view.elemSize());
	update();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.23
	Description	: 选择合适的色卡用于增强灰度图像显示 & 槽函数
 *****************************************************************************/

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.08
	Description	: 开启实时红外诊断显示 & 槽函数
 *****************************************************************************/
void XView::trackFeature()
{
	std::vector<uchar> status;
	std::vector<float> errors;
	//计算稀疏特征集的光流
	calcOpticalFlowPyrLK(pr_gray, gray, fpts[0], fpts[1], status, errors);
	int k = 0;
	for (int i = 0; i < fpts[1].size(); i++)
	{
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);
		if (dist > 2 && status[i])
		{
			inPoints[k] = inPoints[i];
			fpts[1][k] = fpts[1][i];
			k++;
		}
	}
	inPoints.resize(k);
	fpts[1].resize(k);
	//绘制光流轨迹
	RNG rng(0);
	for (int i = 0; i < fpts[0].size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		line(view, inPoints[i], fpts[1][i], color, 2);
		circle(view, fpts[1][i], 2, Scalar(0, 255, 255), 2);
	}
	std::swap(fpts[1], fpts[0]);
}


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
    cv::resize(mat, mat, Size(576, 768));

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

	//=======================================================================
#if 0
	cvtColor(view, gray, COLOR_BGR2GRAY);

	for (int i = 0; i<gray.rows; ++i) {
		for (int j = 0; j<gray.cols; ++j) {
			if (gray.at<uchar>(i, j)>128)
				gray.at<uchar>(i, j) = 0;
			else
				gray.at<uchar>(i, j) = 255;
		}
	}
	imshow("背景加强", gray);

	//开始进行强角点检测  
	//先配置需要的函数参数  
	vector<Point2f> dstPoint2f;
	//void cv::goodFeaturesToTrack(InputArray _image, OutputArray _corners,
	//	int maxCorners, double qualityLevel, double minDistance,
	//	InputArray _mask, int blockSize,
	//	bool useHarrisDetector, double harrisK)
	goodFeaturesToTrack(gray, dstPoint2f, 100, 0.01, 10, Mat(), 3);

	//遍历每个点，进行绘制，便于显示  
	Mat dstImage;
	view.copyTo(dstImage);
	for (int i = 0; i < (int)dstPoint2f.size(); i++) {
		circle(dstImage, dstPoint2f[i], 1, Scalar(theRNG().uniform(0, 255), 
			theRNG().uniform(0, 255), theRNG().uniform(0, 255)) , 2, 8);
	}

	imshow("【检测到的角点图】", dstImage);
#endif
	//=======================================================================

	//const struct_State * pState;
	//pState = XDev::Get()->get_mgdevice()->GetFrameStatisticalData();
	//QString str = QString::fromLocal8Bit("最高温度 %1, 最低温度 %2, 平均温度 %3").
	//	arg(pState->intMaxTemperature * 0.001, 2, 'f', 2).
	//	arg(pState->intMinTemperature * 0.001, 2, 'f', 2).
	//	arg(pState->intAveTemperature * 0.001, 2, 'f', 2);
	//Temp->setText(str);

	//putText(cmMat, std::to_string(pState->intMaxTemperature * 0.001), Point(0, 30),
	//	cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, 8);
	//putText(cmMat, std::to_string(pState->intAveTemperature * 0.001), Point(0, 370),
	//	cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, 8);
	//putText(cmMat, std::to_string(pState->intMinTemperature * 0.001), Point(0, 770),
	//	cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, 8);

    //	memcpy(cm.bits(), cmMat.data, cmMat.rows * cmMat.cols * cmMat.elemSize());
    //imshow("test photo", view);
	update();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.28play
	Description	: 暂停实时红外诊断显示 & 槽函数
 *****************************************************************************/
void XView::pause()
{
	//DealFrame::Get()->set_pause(true);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.06.06
	Description	: 拍照 & 槽函数
 *****************************************************************************/
void XView::photoBody()
{
	QDateTime currentTime = QDateTime::currentDateTime();
    QString current_date = PICTURE_BODY_PATH + currentTime.toString("yyyy_MM_dd_hh_mm_ss") + ".png";
    Mat saveMat;
    //open以bgr排列储存数据,故需要转化后保存
    cvtColor(view, saveMat, CV_BGR2RGB);
	//因需求改变,直接保存彩色图
    imwrite(current_date.toStdString(), saveMat);
}

void XView::photoBreast()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString current_date = PICTURE_BREAST_PATH + currentTime.toString("yyyy_MM_dd_hh_mm_ss") + ".png";
    Mat saveMat;
    //open以bgr排列储存数据,故需要转化后保存
    cvtColor(view, saveMat, CV_BGR2RGB);
    //因需求改变,直接保存彩色图
    imwrite(current_date.toStdString(), saveMat);
}

void XView::set_cmAlpha(int val)
{
	XPro::Get()->set_cmAlpha(val);
}
void XView::set_cmBeta(int val)
{
	XPro::Get()->set_cmBeta(val);
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
		PrInfo("使用自定义模板");
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

cv::Mat XView::get_cmTable()
{
    return cmTable;
}
