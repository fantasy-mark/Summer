/*
����:
LogSys Log(LogLevel_Info, LogSys::GetAppPathA().append("log\\"));
#define pr(fmt) do { \
	Log.Info("%s  %s  %d\n\t" fmt, __FILE__, __FUNCTION__, __LINE__); \
} while(0);

in the  function:
	pr("I'm a log trace");

show:
Info	2018-05-29 14:18:22
c:\users\administrator\desktop\svn\irexam\src\ui\irexam.cpp  IrExam::IrExam  41
	i'm a log trace
tips:
#define __RELEASE__		 //���ڹرմ�ӡ��Ϣ
*/

#ifndef _LOGSYS_H_
#define _LOGSYS_H_
#include <Windows.h>
#include <stdio.h>
#include <string>

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: ���ڿ�ݴ�ӡ����λ�� & �� (��GNU C�﷨�е㲻ͬ)
 *****************************************************************************/
#define PrFatal(fmt, ...) do { \
	LogSys::Get()->Fatal("%s  %s  %d\n\t" fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

#define PrError(fmt, ...) do { \
	LogSys::Get()->Error("%s  %s  %d\n\t" fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

#define PrWarning(fmt, ...) do { \
	LogSys::Get()->Warning("%s  %s  %d\n\t" fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

#define PrInfo Pr
#define Pr(fmt, ...) do { \
	LogSys::Get()->Info("%s  %s  %d\n\t" fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);


//��־�������ʾ��Ϣ  
static const std::string strFatalPrefix = "Fatal\t";
static const std::string strErrorPrefix = "Error\t";
static const std::string strWarningPrefix = "Warning\t";
static const std::string strInfoPrefix = "Info\t";

//��־����ö��  
typedef enum {
	LogLevel_Stop = 0,  //ʲô������¼  
	LogLevel_Fatal,     //ֻ��¼���ش���  
	LogLevel_Error,     //��¼���ش�����ͨ����  
	LogLevel_Warning,   //��¼���ش�����ͨ���󣬾���  
	LogLevel_Info       //��¼���ش�����ͨ���󣬾��棬��ʾ��Ϣ(Ҳ����ȫ����¼)  
} LogLevel;

class LogSys {
public:
	static LogSys * Get();
	//��������
	virtual ~LogSys();
public:
	//д���ش�����Ϣ  
	void Fatal(const char *lpcszFormat, ...);
	//д������Ϣ  
	void Error(const char *lpcszFormat, ...);
	//д������Ϣ 
	void Warning(const char *lpcszFormat, ...);
	//д��ʾ��Ϣ  
	void Info(const char *lpcszFormat, ...);
	//�ı�д��־����  
	void ChangeLogLevel(LogLevel nLevel);
	//��ȡ��������·��  
	static std::string GetAppPathA();
	//��ʽ���ַ���  
	static std::string FormatString(const char *lpcszFormat, ...);
private:
	//д�ļ�����  
	void Trace(const std::string &strLog);
	//��ȡ��ǰϵͳʱ��  
	std::string GetTime();
	//�ļ�ȫ·���õ��ļ���  
	const char *path_file(const char *path, char splitter);
protected:
	//nLogLevel����־��¼�ĵȼ����ɿ�  
	//strLogPath����־Ŀ¼���ɿ�  
	//strLogName����־���ƣ��ɿ�  
	LogSys(LogLevel nLogLevel = LogLevel::LogLevel_Info, const std::string strLogPath = "", const std::string strLogName = "");
	//д��־�ļ���  
	FILE * m_pFileStream;
	//д��־����  
	LogLevel m_nLogLevel;
	//��־Ŀ¼  
	std::string m_strLogPath;
	//��־������
	std::string m_strLogName;
	//��־�ļ�ȫ·��  
	std::string m_strLogFilePath;
	//�߳�ͬ�����ٽ�������  
	CRITICAL_SECTION m_cs;
};

#endif //_LOGSYS_