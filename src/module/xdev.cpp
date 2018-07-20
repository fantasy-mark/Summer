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
    Date		: 2018.07.18
    Description	: �豸�������鹹����
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
	Description	: ���Ӻ���ͼ��
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
    Description	: ���ڷ���ͼ��֡
 *****************************************************************************/
void CALLBACK XDev::newFrame(UINT intChannelIndex, int intCameraTemperature,
    DWORD dwFFCCounterdown, DWORD dwCamState,
    DWORD dwStreamType, void * dwUser)
{
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(dwFFCCounterdown);
    Q_UNUSED(dwStreamType);
    Q_UNUSED(dwUser);

    //TODO �˴���ʼ������ʾ֡
    emit magFrame(intCameraTemperature, dwCamState);
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ����ͼ������
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
               //            m_CamInfo->intVideoHeight, 16, m_CamInfo->intVideoHeight  };	//����5\6Ϊɫ�����Ŀ��

    //DWORD streamType = STREAM_TEMPERATURE;
    //return MAG_StartProcessImage(channelIndex, &paraOut, newFrame, streamType, (void *)this);
    return true;
}
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
