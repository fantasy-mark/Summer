#include "xdev.h"
#include "xserial.h"

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.18
	Description	: �豸�����๹�캯��
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
	Description	: �豸�������鹹����
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
    Description	: �豸�������鹹����
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

    //TODO �˴���ʼ������ʾ֡
    if (isBMPData) {
        struct_CamInfo cameraInfo = XDev::Get()->get_cameraInfo();
        Mat postMat = Mat(cameraInfo.intVideoHeight, cameraInfo.intVideoWidth, CV_8UC1);
        memcpy(postMat.data, pData, cameraInfo.intVideoWidth * cameraInfo.intVideoHeight);
        //ԭʼͼ��
        rotate(postMat, postMat, ROTATE_180);
        emit XDev::Get()->magFrame(postMat);
    }

    //������һ֡
    MAG_TransferPulseImage(XDev::Get()->channelIndex);
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
    if (MAG_IsProcessingImage(channelIndex))
        return true;

    MAG_GetCamInfo(channelIndex, &m_CamInfo, sizeof(m_CamInfo));
    OutputPara paraOut = { m_CamInfo.intFPAWidth, m_CamInfo.intFPAHeight, m_CamInfo.intVideoWidth,
                           m_CamInfo.intVideoHeight, 16, m_CamInfo.intVideoHeight  };	//����5\6Ϊɫ�����Ŀ��

    DWORD streamType = STREAM_TEMPERATURE;

    return MAG_StartProcessImage(channelIndex, &paraOut, newFrame, streamType, (void *)this);
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
void XDev::auto_focus()
{
    MAG_SetPTZCmd(channelIndex, PTZFocusAuto, 0);
}
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
void XDev::up_cradle()
{
    button_state = 1;
}
/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.17
    Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::stop_cradle()
{
    button_state = 0;
}
/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.17
	Description	: ���ٺ���ͼ��ʵ��
 *****************************************************************************/
void XDev::down_cradle()
{
    button_state = 2;
}
