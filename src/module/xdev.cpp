#include "xdev.h"

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: �豸�����๹�캯��
 *****************************************************************************/
XDev::XDev()
{
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: �豸�������鹹����
 *****************************************************************************/
XDev::~XDev()
{
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ��������ͼ��ʵ��
 *****************************************************************************/
bool XDev::create_irDev()
{
	//�Զ�����
	MAG_EnableAutoReConnect(true);
	//����ͨ��
	if (!MAG_IsChannelAvailable(channelIndex)) {
		if (!MAG_NewChannel(channelIndex))
			return false;
	}
	//��ʼ��ͨ����Դ
	if (!MAG_IsInitialized(channelIndex)) {
		return MAG_Initialize(channelIndex, NULL);
	}

	return true;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ˢ�º���ͼ��
 *****************************************************************************/
void XDev::refresh_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���Ӻ���ͼ��
 *****************************************************************************/
void XDev::connect_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ����ͼ������
 *****************************************************************************/
void XDev::play_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ����ͼ������
 *****************************************************************************/
void XDev::photo_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ����ͼ��ֹͣ
 *****************************************************************************/
void XDev::stop_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: �Ͽ�����ͼ������
 *****************************************************************************/
void XDev::disconnect_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::free_irDev() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::auto_focus() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::near_focus() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::far_focus() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::up_cradle() {}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::down_cradle() {}
