#include "xpro.h"
#include <qDebug>

XPro::XPro()
{
}

XPro::~XPro()
{
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ��ȡͼ����ʵ��
 *****************************************************************************/
XPro* XPro::Get()
{
	static XPro xp;
	return &xp;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ����srcMat��desMat
 *****************************************************************************/
void XPro::setMat(cv::Mat srcMat)
{
	if (srcMat.empty()) return;
	srcMat.copyTo(desMat);
}

/*****************************************************************************
Copyright	: Yaqian Group
Author		: Mark_Huang ( hacker.do@163.com )
Date		: 2018.05.14
Description	: ��ȡdesMat��srcMat
*****************************************************************************/
void XPro::getMat(cv::Mat srcMat)
{
	if (desMat.empty()) return;
	desMat.copyTo(srcMat);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ����ͼ��ĶԱȶȺ�����
 *****************************************************************************/
void XPro::setGain()
{
	if (desMat.empty()) return;
	desMat.convertTo(desMat, -1, this->alpha, this->beta);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ���öԱȶ� & �ۺ���
 *****************************************************************************/
void XPro::setAlpha(double alpha)
{
	if (desMat.empty()) return;
	this->alpha = alpha;
}

/*****************************************************************************
Copyright	: Yaqian Group
Author		: Mark_Huang ( hacker.do@163.com )
Date		: 2018.05.14
Description	: �������� & �ۺ���
*****************************************************************************/
void XPro::setBeta(double beta)
{
	if (desMat.empty()) return;
	this->beta = beta;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ֱ��ͼ���⻯
 *****************************************************************************/
void XPro::equalizeHist()
{
	cv::equalizeHist(desMat, desMat);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.14
	Description	: ʹ��Canny���ӱ�Ե���
 *****************************************************************************/
void XPro::edgeCanny()
{
	cv::Canny(desMat, desMat, 50, 120, 3);
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.23
	Description	: ѡ��ʺ�ģ��(HSV)���ڻҶ�ӳ��
 *****************************************************************************/
void XPro::rainbowColorMap()
{
	IplImage * lookUpTable = cvCreateImage(cvSize(256, 1), IPL_DEPTH_8U, 3);

	colorMap = Mat(1, 256, CV_32FC3);
	//printf(" ����          H     V     S\n");
	qDebug() << cm_alpha << cm_beta;
	for (int gray = 0; gray < MAXGRAY + 1; gray++) {
		float val = gray / cm_alpha;
		val = val * 360. / 250. + cm_beta;
		if (val > 360.) {
			val -= 360.;
		}
		//���ȡ����ȶ��Ӿ��й�Ӱ��С�����ù̶�ֵ
		colorMap.at<Vec3f>(0, gray) = Vec3f(val, 0.5, 1.);
		//printf("gray %3d -> %6.2f %6.2f %6.2f\n", gray, colorMap.at<Vec3f>(0, gray)[0],
		//	colorMap.at<Vec3f>(0, gray)[1], colorMap.at<Vec3f>(0, gray)[2]);
	}

	cvtColor(colorMap, colorMap, COLOR_HLS2BGR);
	IplImage Ipl32F(colorMap);
	//cvConvertScaleʹ��Mat��Ϊ����ִ�л����
	cvConvertScale(&Ipl32F, lookUpTable, 255, 0);
	//ͨ��LUT(grayMat1C, colorMap3C, colorMat3C)ʹ��ģ��
	colorMap = cvarrToMat(lookUpTable);
}

void XPro::set_cmAlpha(int val)
{
	cm_alpha = val / 100.;
	return;
}
void XPro::set_cmBeta(int val)
{
	cm_beta = val / 1.;
	return;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.23
	Description	: ���colorMap
 *****************************************************************************/
cv::Mat XPro::get_colormap(void)
{
	return colorMap;
}