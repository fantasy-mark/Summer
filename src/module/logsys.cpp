#include <time.h>  
#include <stdarg.h>  
#include <direct.h>  
#include <vector>  
#include <shlobj.h>
#include <io.h>
#include <iostream>

#include "logsys.h"  

using std::cout;
using std::string;
using std::vector;

#define SEPARATOR	"\\"
#define SEPARATOR_	'\\'

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.28
	Description	: �����༶Ŀ¼
 *****************************************************************************/
void CreateDir(const char *dir)
{
	int m = 0, n;
	string str1, str2;

	str1 = dir;
	str2 = str1.substr(0, 2);
	str1 = str1.substr(3, str1.size());

	while (m >= 0) {
		m = str1.find(SEPARATOR);

		str2 += SEPARATOR + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //�жϸ�Ŀ¼�Ƿ����
		if (n == -1) {
			_mkdir(str2.c_str());     //����Ŀ¼
		}

		str1 = str1.substr(m + 1, str1.size());
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: ���캯��, ��ʼ��LogSysʵ��
 *****************************************************************************/
LogSys::LogSys(LogLevel nLogLevel, const std::string strLogPath, const std::string strLogName)
	:m_nLogLevel(nLogLevel),
	m_strLogPath(strLogPath),
	m_strLogName(strLogName)
{
	m_pFileStream = NULL;
	if (m_strLogPath.empty()) {
		m_strLogPath = GetAppPathA();
	}
	if (m_strLogPath[m_strLogPath.length() - 1] != SEPARATOR_) {
		m_strLogPath.append(SEPARATOR);
	}
	//�����ļ���
	CreateDir(m_strLogPath.c_str());
	//������־�ļ�  
	if (m_strLogName.empty()) {
		time_t curTime;
		time(&curTime);
		tm tm1;
		localtime_s(&tm1, &curTime);
		//��־�������磺20160101_2130.log 
		m_strLogName = FormatString("%04d%02d%02d_%02d%02d.md", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
	}
	m_strLogFilePath = m_strLogPath.append(m_strLogName);

	//��׷�ӵķ�ʽ���ļ���  
	fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");

	InitializeCriticalSection(&m_cs);
}


/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: ���ڻ����ʵ��, ����LogSys::Get()->ChangeLogLevel(LogLevel::LogLevel_Error);
 *****************************************************************************/
LogSys * LogSys::Get()
{
	static LogSys Log(LogLevel_Info, LogSys::GetAppPathA().append("log\\"));
	return &Log;
}

//��������  
LogSys::~LogSys()
{
	//�ͷ��ٽ���  
	DeleteCriticalSection(&m_cs);
	//�ر��ļ���  
	if (m_pFileStream)
	{
		fclose(m_pFileStream);
		m_pFileStream = NULL;
	}
}

//�ļ�ȫ·���õ��ļ���  
const char *LogSys::path_file(const char *path, char splitter)
{
	return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
}

//д���ش�����Ϣ ������Ϣ
void LogSys::Fatal(const char *lpcszFormat, ...)
{
	//�жϵ�ǰ��д��־����  
	if (LogLevel::LogLevel_Fatal > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������  
		std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //���ñ�������  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
    if (strResult.empty())
	{
		return;
	}
    string strLog = strFatalPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;font-size:24px;color:#ff0000\">" + strResult + "</div>\n");

	//д��־�ļ�  
	Trace(strLog);
}

//д������Ϣ  ִ��ʧ����Ϣ
void LogSys::Error(const char *lpcszFormat, ...)
{
	//�жϵ�ǰ��д��־����  
	if (LogLevel::LogLevel_Error > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������  
		std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //���ñ�������  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
    if (strResult.empty()) {
		return;
	}
	string strLog = strErrorPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;font-size:18px;color:#ff5151\">" + strResult + "</div>\n");

	//д��־�ļ�  
	Trace(strLog);
}

//д������Ϣ  ����Ԥ��֮�����Ϣ
void LogSys::Warning(const char *lpcszFormat, ...)
{
	//�жϵ�ǰ��д��־����  
	if (LogLevel::LogLevel_Warning > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������  
		std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //���ñ�������  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
    if (strResult.empty()) {
		return;
	}
	string strLog = strWarningPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;color:#ff8f00\">" + strResult + "</div>\n");

	//д��־�ļ�  
	Trace(strLog);
}


//дһ����Ϣ ��ʾ��Ϣ
void LogSys::Info(const char *lpcszFormat, ...)
{
    //�жϵ�ǰ��д��־���� mark fixme ��ǰ�����ӡ������¼
    //if (LogLevel::LogLevel_Info > m_nLogLevel)
    //	return;
	string strResult;
	if (NULL != lpcszFormat) {
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������  
		std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //���ñ�������  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
#if 0
    if (strResult.empty()) {
		return;
	}
	string strLog = strInfoPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;color:#00ff00\">" + strResult + "</div>\n");

	//д��־�ļ�  
	Trace(strLog);
#endif
}

//��ȡϵͳ��ǰʱ��  
string LogSys::GetTime()
{
	time_t curTime;
	time(&curTime);
	tm tm1;
	localtime_s(&tm1, &curTime);
	//2016-01-01 21:30:00  
	string strTime = FormatString("%04d-%02d-%02d %02d:%02d:%02d ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

	return strTime;
}

//�ı�д��־����  
void LogSys::ChangeLogLevel(LogLevel nLevel)
{
	m_nLogLevel = nLevel;
}

//д�ļ�����  
void LogSys::Trace(const string &strLog)
{
	try {
		//�����ٽ���  
		EnterCriticalSection(&m_cs);
		//���ļ���û�д򿪣������´�  
		if (NULL == m_pFileStream) {
			fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
			if (!m_pFileStream) {
				return;
			}
		}
        //д��־��Ϣ���ļ���
        fprintf(m_pFileStream, "\n%s\n", strLog.c_str());
		fflush(m_pFileStream);
		//�뿪�ٽ���  
		LeaveCriticalSection(&m_cs);
	}
	//�������쳣�������뿪�ٽ�������ֹ����  
    catch (...) {
		LeaveCriticalSection(&m_cs);
	}
}

string LogSys::GetAppPathA()
{
	char szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, sizeof(szFilePath));
	_splitpath_s(szFilePath, szDrive, szDir, szFileName, szExt);

	string str(szDrive);
	str.append(szDir);
	return str;
}

string LogSys::FormatString(const char *lpcszFormat, ...)
{
	string strResult;
	if (NULL != lpcszFormat) {
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������  
		std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //���ñ�������  
	}
	return strResult;
}
