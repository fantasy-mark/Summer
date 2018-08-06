#include "xserial.h"
#include <qDebug>

XSerial::XSerial()
{
    open_serial();
}

XSerial::~XSerial()
{
    close_serial();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: ʹ�þ�̬��,��ö���ָ��
 *****************************************************************************/
XSerial * XSerial::Get()
{
	static XSerial xs;
	return &xs;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: ɨ����ô��ڲ���ȡ
 *****************************************************************************/
QStringList XSerial::scan_serial()
{
	QStringList serialList;

	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString id;
        id.sprintf("-%04x:%04x", info.productIdentifier(), info.vendorIdentifier());
        serialList.append(info.portName() + id);
    }

	return serialList;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: ����̨��ҡ�˴������
 *****************************************************************************/
bool XSerial::open_serial()
{
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		int _vid, _pid;
		QSerialPort * serial_p = new QSerialPort;
		/* ����Ӳ����Ϣ */
		serial_p->setPort(info);
		/* �ж϶˿��Ƿ��ܴ� */
		if (serial_p->open(QIODevice::ReadWrite)) {
            if (info.hasVendorIdentifier())
                _vid = info.vendorIdentifier();
            if (info.hasProductIdentifier())
                _pid = info.productIdentifier();
			//Ϊָ���豸���ò�����
			if (_vid == POLE_VID && _pid == POLE_PID && poleSerial == NULL) {
				poleSerial = serial_p;
				poleSerial->setBaudRate(9600);
			} else if (_vid == CRADLE_VID && _pid == CRADLE_PID && cradleSerial == NULL) {
                cradleSerial = serial_p;
				cradleSerial->setBaudRate(115200);
			} else {
				serial_p->close();
				delete serial_p;
				continue;
			}
			/* ��������λ�� */
			serial_p->setDataBits(QSerialPort::Data8);
			/* ������żУ�� */
			serial_p->setParity(QSerialPort::NoParity);
			/* ����ֹͣλ */
			serial_p->setStopBits(QSerialPort::OneStop);
			/* ���������� */
			serial_p->setFlowControl(QSerialPort::NoFlowControl);
			/* ���ö�����,0Ϊȫ���� */
			serial_p->setReadBufferSize(0);
        }
	}
	if (cradleSerial != NULL && poleSerial != NULL)
		return true;
	else
		return false;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: �ر���̨��ҡ�˴���
 *****************************************************************************/
void XSerial::close_serial()
{
	if (cradleSerial) {
		cradleSerial->close();
		cradleSerial = NULL;
	}
	if (poleSerial) {
		poleSerial->close();
		poleSerial = NULL;
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: ���ҡ���ٶ�
 *****************************************************************************/
bool XSerial::get_poleSpeed(int * speed)
{
    if (cradleSerial == NULL)
        return false;
    if (! poleSerial->isOpen()) {
		return false;
	}
    QByteArray data;
	//1 �ȴ���
    poleSerial->waitForReadyRead(10);
	//��ϳ�֡��Ҫ2*9-1=17,������������
	int len = poleSerial->bytesAvailable();
	if (len > 17) {
		poleSerial->read(len - 17);
	}
    data = poleSerial->readAll();
    if (data.size() < 17)
        return false;
	int begin;
	//�ҵ�����֡���ֽ�ͷ 0xff
	for (int i = 8; i >= 0; i--) {
		if ((unsigned char)data.at(i) == 0xff) {
			begin = i;
			break;
		}
	}
	//ȡ����֡
	unsigned char bytes[9];
	for (int i = 0; i < 9; i++) {
		bytes[i] = (unsigned char)data.at(begin + i);
	}
	//У�鲢����ٶ�
	if (bytes[8] == bytes[1] + bytes[2] + bytes[3] + bytes[4] + bytes[5] + bytes[6] + bytes[7]) {
		*speed = ((int)bytes[1] << 8 | (int)bytes[2]) - 0x200;
		return true;
	}

	return false;
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
    Description	: ������̨
 *****************************************************************************/
bool XSerial::set_cradleSpeed(int *speed)
{
    if (cradleSerial == NULL)
        return false;
    if (! cradleSerial->isOpen()) {
		return false;
	}
    //Ĭ�Ϸ�ת\��ֹ
	static const char rdata[] = { 0xff, 0x1f, 0x10, 0x08, 0x20, 0x00, 0x56 };
	QByteArray data = QByteArray::fromRawData(rdata, sizeof(rdata));
	//��ת,�ٶ�0x10,У��
    if (*speed > 0x100 && *speed < 0x200) {
        data[3] = 0x08;
        data[5] = 0x10;
        data[6] = 0x66;
    } else  if (*speed > -0x200 && *speed < -0x100) {
    //��ת,�ٶ�0x10,У��
        data[3] = 0x10;
        data[5] = 0x10;
        data[6] = 0x6e;
    } else {
        data[3] = 0x08;
        data[5] = 0x00;
        data[6] = 0x56;
    }

	if (data.size() == cradleSerial->write(data)) {
		//������Ч?
		//cradleSerial->flush();
		return true;
	}
	return false;
}
