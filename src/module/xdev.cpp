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
void XDev::refresh_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 连接红外图传
 *****************************************************************************/
void XDev::connect_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: 红外图传播放
 *****************************************************************************/
void XDev::play_irDev() {}
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
