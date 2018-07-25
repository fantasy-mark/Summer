#pragma once

#include <QObject>
#include <opencv2\opencv.hpp>

using namespace cv;

#define		MAXGRAY		255

class XPro : public QObject
{
	Q_OBJECT

public:
	~XPro();
	static XPro * Get();
	void setMat(cv::Mat srcMat);
	void getMat(cv::Mat srcMat);
	//设置亮度和对比度 0~100 1.0~3.0
	void setGain();
	void equalizeHist();
	void edgeCanny();
	void rainbowColorMap();
	cv::Mat get_colormap();
	void set_cmAlpha(int val);
	void set_cmBeta(int val);

public slots:
	void setAlpha(double alpha);
	void setBeta(double beta);

protected:
	XPro();
	cv::Mat desMat;
	cv::Mat colorMap;
	float cm_alpha = 1.1, cm_beta = 180.0;
	double alpha = 1, beta = 0;
};
