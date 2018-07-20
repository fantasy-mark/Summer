#include "xdev.h"

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: 设备控制类构造函数
 *****************************************************************************/
XDev::XDev()
{
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: 设备控制类虚构函数
 *****************************************************************************/
XDev::~XDev()
{
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
bool XDev::refresh_irDev()
{
    std::vector<struct_TerminalList> m_vTermList;

    if (!MAG_IsInitialized(channelIndex)) {
        return false;
    }
    if (!MAG_IsLinked(channelIndex)) {
        if(!MAG_LinkCamera(channelIndex, ip, timeout)) {
            return false;
        }
    }
    if (!MAG_EnumCameras()) {
        return false;
    }
    Sleep(200);
    int n = MAG_GetTerminalList(NULL, 0);
    m_vTermList.resize(n);

    QStringList list;
    for (int i = 0; i < n; i++) {
        list << QString(m_vTermList[i].charTerminalName);
    }

    emit devList(list);

    return true;
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
void CALLBACK XDev::newFrame(UINT intChannelIndex, int intCameraTemperature,
    DWORD dwFFCCounterdown, DWORD dwCamState,
    DWORD dwStreamType, void * dwUser)
{
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(dwFFCCounterdown);
    Q_UNUSED(dwStreamType);
    Q_UNUSED(dwUser);

    //TODO 此处开始处理显示帧
    emit magFrame(intCameraTemperature, dwCamState);
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

    //TODO
    //MAG_GetCamInfo(channelIndex, &m_CamInfo, sizeof(&m_CamInfo));
    //const struct_CamInfo camInfo = Get_CamInfo();
    //OutputPara paraOut = { m_CamInfo->intFPAWidth, m_CamInfo->intFPAHeight, m_CamInfo->intVideoWidth,
               //            m_CamInfo->intVideoHeight, 16, m_CamInfo->intVideoHeight  };	//参数5\6为色卡条的宽高

    //DWORD streamType = STREAM_TEMPERATURE;
    //return MAG_StartProcessImage(channelIndex, &paraOut, newFrame, streamType, (void *)this);
    return true;
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
void XDev::auto_focus() {}
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
void XDev::up_cradle() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 销毁红外图传实例
 *****************************************************************************/
void XDev::down_cradle() {}
