#include "xdev.h"
#include "xserial.h"

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: 设备控制类构造函数
 *****************************************************************************/
XDev::XDev()
{
    create_irDev();

    m_pThread = new QThread(this);
    m_pTimer = new QTimer();
    m_pTimer->moveToThread(m_pThread);
    m_pTimer->setInterval(100);
    connect(m_pThread, SIGNAL(started()), m_pTimer, SLOT(start()));
    connect(m_pTimer, &QTimer::timeout, this, &XDev::timeOutSlot, Qt::DirectConnection);
    //begin thread to run
    m_pThread->start();
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: 设备控制类虚构函数
 *****************************************************************************/
XDev::~XDev()
{
    m_pThread->quit();
    m_pThread->wait();
    //m_pThread->destroyed();

    MAG_StopProcessImage(channelIndex);
    MAG_DisLinkCamera(channelIndex);
    MAG_Free(channelIndex);
    MAG_DelChannel(channelIndex);
}

void XDev::run()
{
    m_pTimer->start(1000);
}


void XDev::timeOutSlot()
{
    if (XSerial::Get()->get_poleSpeed(&cradle_speed)) {
        if (cradle_speed > 0x100 || cradle_speed < -0x100) {
                XSerial::Get()->set_cradleSpeed(&cradle_speed);
                return;
        }
    }
    if (button_state == 1) {
        cradle_speed = 0x180;
        XSerial::Get()->set_cradleSpeed(&cradle_speed);
    } else if (button_state == 2) {
        cradle_speed = -0x180;
        XSerial::Get()->set_cradleSpeed(&cradle_speed);
    } else {
        cradle_speed = 0x0;
        XSerial::Get()->set_cradleSpeed(&cradle_speed);
    }
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.18
    Description	: 设备控制类虚构函数
 *****************************************************************************/
XDev * XDev::Get()
{
    static XDev xd;
    return &xd;
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.18
    Description	:
 *****************************************************************************/
struct_CamInfo XDev::get_cameraInfo()
{
    return m_CamInfo;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 创建红外图传实例
 *****************************************************************************/
bool XDev::create_irDev()
{
	//自动重连
	MAG_EnableAutoReConnect(true);
	//创建通道
	if (!MAG_IsChannelAvailable(channelIndex)) {
		if (!MAG_NewChannel(channelIndex))
			return false;
	}
	//初始化通道资源
	if (!MAG_IsInitialized(channelIndex)) {
		return MAG_Initialize(channelIndex, NULL);
	}

	return true;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 刷新红外图传
 *****************************************************************************/
QStringList XDev::refresh_irDev()
{
    QStringList list = {};
    std::vector<struct_TerminalList> m_vTermList;

    if (!MAG_IsInitialized(channelIndex)) {
        return { "No Init" };
    }
#if 0
    if (!MAG_IsLinked(channelIndex)) {
        if(!MAG_LinkCamera(channelIndex, ip, timeout)) {
            return { "No Link" };
        }
    }
#endif
    if (!MAG_EnumCameras()) {
        return { "No Enum" };
    }
    Sleep(200);
    int n = MAG_GetTerminalList(NULL, 0);
    m_vTermList.resize(n);

    for (int i = 0; i < n; i++) {
        MAG_GetTerminalList(&m_vTermList[i], sizeof(m_vTermList[i]));
        list << QString(m_vTermList[i].charTerminalName);
    }
    return list;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 连接红外图传
 *****************************************************************************/
bool XDev::connect_irDev()
{
    if (!MAG_IsInitialized(channelIndex)) {
        return false;
    }
    if (!MAG_IsLinked(channelIndex)) {
        if(!MAG_LinkCamera(channelIndex, ip, timeout)) {
            return false;
        }
    }
    return true;
}
/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.06.06
    Description	: 用于发送图像帧
 *****************************************************************************/
void CALLBACK newFrame(UINT intChannelIndex, int intCameraTemperature,
    DWORD dwFFCCounterdown, DWORD dwCamState,
    DWORD dwStreamType, void * dwUser)
{
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(intCameraTemperature);
    Q_UNUSED(dwFFCCounterdown);
    Q_UNUSED(dwCamState);
    Q_UNUSED(dwStreamType);
    Q_UNUSED(dwUser);

    MAG_LockFrame(XDev::Get()->channelIndex);

    const UCHAR * pData = NULL;
    const BITMAPINFO * pInfo = NULL;

    BOOL isBMPData = MAG_GetOutputBMPdata(XDev::Get()->channelIndex, &pData, &pInfo);

    MAG_UnLockFrame(XDev::Get()->channelIndex);

    //TODO 此处开始处理显示帧
    if (isBMPData) {
        struct_CamInfo cameraInfo = XDev::Get()->get_cameraInfo();
        Mat postMat = Mat(cameraInfo.intVideoHeight, cameraInfo.intVideoWidth, CV_8UC1);
        memcpy(postMat.data, pData, cameraInfo.intVideoWidth * cameraInfo.intVideoHeight);
        //原始图像
        rotate(postMat, postMat, ROTATE_180);
        emit XDev::Get()->magFrame(postMat);
    }

    //处理下一帧
    MAG_TransferPulseImage(XDev::Get()->channelIndex);
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 红外图传播放
 *****************************************************************************/
bool XDev::play_irDev()
{
    if (!MAG_IsInitialized(channelIndex)) {
        return false;
    }
    if (!MAG_IsLinked(channelIndex)) {
        return false;
    }
    if (MAG_IsProcessingImage(channelIndex))
        return true;

    MAG_GetCamInfo(channelIndex, &m_CamInfo, sizeof(m_CamInfo));
    OutputPara paraOut = { m_CamInfo.intFPAWidth, m_CamInfo.intFPAHeight, m_CamInfo.intVideoWidth,
                           m_CamInfo.intVideoHeight, 16, m_CamInfo.intVideoHeight  };	//参数5\6为色卡条的宽高

    DWORD streamType = STREAM_TEMPERATURE;

    return MAG_StartProcessImage(channelIndex, &paraOut, newFrame, streamType, (void *)this);
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 红外图传拍照
 *****************************************************************************/
void XDev::photo_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 红外图传停止
 *****************************************************************************/
void XDev::stop_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 断开红外图传连接
 *****************************************************************************/
void XDev::disconnect_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::free_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::auto_focus()
{
    MAG_SetPTZCmd(channelIndex, PTZFocusAuto, 0);
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::near_focus() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::far_focus() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::up_cradle()
{
    button_state = 1;
}
/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.17
    Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::stop_cradle()
{
    button_state = 0;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::down_cradle()
{
    button_state = 2;
}
