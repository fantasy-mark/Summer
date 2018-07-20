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

#include <QObject>

class XDev : public QObject
{
	Q_OBJECT

public:
	XDev();
	~XDev();
	//��������ͷ - ����
	int channelIndex = 0;
	int ip = 192 | 168 << 8 | 1 << 16 | 118 << 24;
	int timeout = 500;
	//�޸�SDK
	struct_CamInfo m_CamInfo;
	struct_CeRegContent m_RegContent;

public slots:
	//��������ͷ - ͼ��
	bool create_irDev();
	void refresh_irDev();
	void connect_irDev();
	void play_irDev();
	void photo_irDev();
	void stop_irDev();
	void disconnect_irDev();
	void free_irDev();
	//��ͷ�Խ�/��̨����
	void auto_focus();
	void near_focus();
	void far_focus();
	void up_cradle();
	void down_cradle();
};
