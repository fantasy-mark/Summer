#pragma once

#ifdef WIN32
#include "windows.h"
#include "thermo/typedef.h"
#include "thermo/mag_errorcode.h"
#include "thermo/ThermoGroupSDK.h"
#else
#include "magsdk/linux/ThermoGroupSDK.h"
#include <stdlib.h>
#endif

#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QThread>
#include "opencv2/opencv.hpp"

using namespace cv;

class XDev : public QObject
{
	Q_OBJECT

public:
    static XDev * Get();
	~XDev();
	//��������ͷ - ����
	int channelIndex = 0;
	int ip = 192 | 168 << 8 | 1 << 16 | 118 << 24;
	int timeout = 500;

    QMutex mutex;
    QThread * m_pThread = NULL;
    QTimer * m_pTimer = NULL;

    struct_CamInfo get_cameraInfo();

public slots:
    void run();
    void timeOutSlot();
    //��������ͷ - ͼ��
	bool create_irDev();
    QStringList refresh_irDev();
    bool connect_irDev();
    bool play_irDev();
	void photo_irDev();
	void stop_irDev();
	void disconnect_irDev();
	void free_irDev();
	//��ͷ�Խ�/��̨����
	void auto_focus();
	void near_focus();
	void far_focus();
	void up_cradle();
    void stop_cradle();
	void down_cradle();

signals:
    void devList(QStringList list);
    void magFrame(cv::Mat);

private:
    XDev();
    //�޸�SDK
    struct_CamInfo m_CamInfo;
    struct_CeRegContent m_RegContent;
    int button_state = 0;     // 0 stop / 1 up / 2 down
    int cradle_speed = 0x00;
};
