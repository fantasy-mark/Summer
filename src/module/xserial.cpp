#include "xserial.h"
#include <qDebug>

XSerial::XSerial()
{
    //不应该在构造函数中打开串口,应该显式控制串口
}

XSerial::~XSerial()
{
    close_serial();
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: 使用静态类,获得对象指针
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
	Description	: 扫描可用串口并获取
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
	Description	: 有云台或摇杆串口则打开
 *****************************************************************************/
bool XSerial::open_serial()
{
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		int _vid, _pid;
		QSerialPort * serial_p = new QSerialPort;
		/* 设置硬件信息 */
		serial_p->setPort(info);
		/* 判断端口是否能打开 */
		if (serial_p->open(QIODevice::ReadWrite)) {
            if (info.hasVendorIdentifier())
                _vid = info.vendorIdentifier();
            if (info.hasProductIdentifier())
                _pid = info.productIdentifier();
			//为指定设备设置波特率
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
			/* 设置数据位数 */
			serial_p->setDataBits(QSerialPort::Data8);
			/* 设置奇偶校验 */
			serial_p->setParity(QSerialPort::NoParity);
			/* 设置停止位 */
			serial_p->setStopBits(QSerialPort::OneStop);
			/* 设置流控制 */
			serial_p->setFlowControl(QSerialPort::NoFlowControl);
			/* 设置读缓冲,0为全缓冲 */
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
	Description	: 关闭云台和摇杆串口
 *****************************************************************************/
void XSerial::close_serial()
{
    if (cradleSerial) {
        if (cradleSerial->isOpen())
            cradleSerial->close();
		cradleSerial = NULL;
	}
    if (poleSerial) {
        if (poleSerial->isOpen())
            poleSerial->close();
		poleSerial = NULL;
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.07.19
	Description	: 获得摇杆速度
 *****************************************************************************/
bool XSerial::get_poleSpeed(int * speed)
{
    if (poleSerial == NULL)
        return false;
    if (! poleSerial->isOpen()) {
		return false;
	}
    QByteArray data;
	//1 等待读
    poleSerial->waitForReadyRead(10);
	//最长合成帧需要2*9-1=17,舍弃多余数据
	int len = poleSerial->bytesAvailable();
	if (len > 17) {
		poleSerial->read(len - 17);
	}
    data = poleSerial->readAll();
    if (data.size() < 17)
        return false;
	int begin;
	//找到完整帧的字节头 0xff
	for (int i = 8; i >= 0; i--) {
		if ((unsigned char)data.at(i) == 0xff) {
			begin = i;
			break;
		}
	}
	//取完整帧
	unsigned char bytes[9];
	for (int i = 0; i < 9; i++) {
		bytes[i] = (unsigned char)data.at(begin + i);
	}
	//校验并获得速度
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
    Description	: 控制云台
 *****************************************************************************/
bool XSerial::set_cradleSpeed(int *speed)
{
    if (cradleSerial == NULL)
        return false;
    if (! cradleSerial->isOpen()) {
		return false;
	}
    //默认反转\静止
	static const char rdata[] = { 0xff, 0x1f, 0x10, 0x08, 0x20, 0x00, 0x56 };
	QByteArray data = QByteArray::fromRawData(rdata, sizeof(rdata));
	//正转,速度0x10,校验
    if (*speed > 0x100 && *speed < 0x200) {
        data[3] = 0x08;
        data[5] = 0x10;
        data[6] = 0x66;
    } else  if (*speed > -0x200 && *speed < -0x100) {
    //反转,速度0x10,校验
        data[3] = 0x10;
        data[5] = 0x10;
        data[6] = 0x6e;
    } else {
        data[3] = 0x08;
        data[5] = 0x00;
        data[6] = 0x56;
    }

	if (data.size() == cradleSerial->write(data)) {
		//立即生效?
		//cradleSerial->flush();
		return true;
	}
	return false;
}
