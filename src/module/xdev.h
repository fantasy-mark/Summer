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
    static XDev * Get();
	~XDev();
	//红外摄像头 - 数据
	int channelIndex = 0;
	int ip = 192 | 168 << 8 | 1 << 16 | 118 << 24;
	int timeout = 500;

    void newFrame(UINT intChannelIndex, int intCameraTemperature,
                  DWORD dwFFCCounterdown, DWORD dwCamState,
                  DWORD dwStreamType, void * dwUser);

    struct_CamInfo get_cameraInfo();

public slots:
	//红外摄像头 - 图传
	bool create_irDev();
    bool refresh_irDev();
    bool connect_irDev();
    bool play_irDev();
	void photo_irDev();
	void stop_irDev();
	void disconnect_irDev();
	void free_irDev();
	//镜头对焦/云台调整
	void auto_focus();
	void near_focus();
	void far_focus();
	void up_cradle();
	void down_cradle();

signals:
    void devList(QStringList list);
    void magFrame(int intCameraTemperature, DWORD dwCamState);

private:
    XDev();
    //巨哥SDK
    struct_CamInfo m_CamInfo;
    struct_CeRegContent m_RegContent;
};
