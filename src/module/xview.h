#pragma once

#include <QFile>
#include <QMutex>
#include <QLabel>
#include <QPoint>
#include <QThread>
#include <QPainter>
#include <QDateTime>
#include <QModelIndex>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <opencv2\opencv.hpp>

using namespace cv;

#define GRAY_PIXEL_WIDTH	1
#define COLOR_PIXEL_WIDTH	3

class XView : public QOpenGLWidget
{
	Q_OBJECT

public:
	XView(QWidget *parent);
	~XView();
	void trackFeature();

Mat pr_gray, gray, pre_frame;
std::vector<Point2f> inPoints;
std::vector<Point2f> fpts[2];


public slots:
	void play(cv::Mat);
	void pause();
    void photoBody();
    void photoBreast();

	void show_image(QString path);
	void select_colormap(QString type);

	void set_cmAlpha(int val);
	void set_cmBeta(int val);

	void set_colormap(int);

protected:
	//����ͼ��
	Mat view;
	//����ɫ��
    Mat cmMat = Mat(768, 32, CV_8UC3);
	//����ӳ���
	Mat cmTable = Mat(256, 1, CV_8UC3);
	//������ʾͼ��
	QImage img;
	//������ʾɫ��
	QImage cm;
	QPainter pen;
	QLabel ql;
private:
	QPoint m_currentPoint;
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	QLabel * Temp;
};

///******************************************************************************
//Copyright	: Yaqian Group
//Author		: Mark_Huang ( hacker.do@163.com )
//Date		: 2018.04.28
//Description	: QObject::moveToThread���QThread::run, ʹ���źŲۻ���
//*****************************************************************************/
//class DealFrame : public QThread
//{
//	Q_OBJECT
//
//public:
//	~DealFrame();
//	static DealFrame * Get();
//	//�߳����
//	void run();
//	void set_photoflag();
//	bool is_pause();
//	void set_pause(bool pause);
//
//	int fps = 0;
//
//signals:
//	void emit_frame(cv::Mat mat);	//signal��������,����mat�ڴ����
//
//protected:
//	DealFrame();
//	QMutex mutex;
//	bool isexit = false;
//	bool photoFlag = false;
//	bool pauseFlag = false;
//};
