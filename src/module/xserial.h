#pragma once

//#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

//��̨��ҡ�˶�Ӧ�Ĵ���
#define POLE_VID	0x067b
#define POLE_PID	0x2303
#define CRADLE_VID	0x1a86
#define CRADLE_PID	0x7523

class XSerial : public QObject
{
	Q_OBJECT

public:
	static XSerial * Get();

public slots:
	//void scan_port();
	//void open_port();
	//void send_data();
	//void sendData1();
	//void sendData2();
	//void sendData3();
	//void RecvMsgEvent();
	//����API
	QStringList scan_serial();
	bool open_serial();
	void close_serial();
	//void send_data();

public slots:
	bool get_poleSpeed(int * speed);
	bool set_cradleSpeed(int *speed);

signals:
	void test();

private:
	XSerial() {};
	QSerialPort * poleSerial = NULL;
	QSerialPort * cradleSerial = NULL;
	/* ҡ�� -0x200~0x200 ��̨-3f~3f, TODO : ���Զ��廮��Ϊ16�� */
	//int Speed = 0;
};
