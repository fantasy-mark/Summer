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
	Description	: 创建多级目录
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
		n = _access(str2.c_str(), 0); //判断该目录是否存在
		if (n == -1) {
			_mkdir(str2.c_str());     //创建目录
		}

		str1 = str1.substr(m + 1, str1.size());
	}
}

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: 构造函数, 初始化LogSys实例
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
	//创建文件夹
	CreateDir(m_strLogPath.c_str());
	//创建日志文件  
	if (m_strLogName.empty()) {
		time_t curTime;
		time(&curTime);
		tm tm1;
		localtime_s(&tm1, &curTime);
		//日志的名称如：20160101_2130.log 
		m_strLogName = FormatString("%04d%02d%02d_%02d%02d.md", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min);
	}
	m_strLogFilePath = m_strLogPath.append(m_strLogName);

	//以追加的方式打开文件流  
	fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");

	InitializeCriticalSection(&m_cs);
}


/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: 用于获得类实例, 比如LogSys::Get()->ChangeLogLevel(LogLevel::LogLevel_Error);
 *****************************************************************************/
LogSys * LogSys::Get()
{
	static LogSys Log(LogLevel_Info, LogSys::GetAppPathA().append("log\\"));
	return &Log;
}

//析构函数  
LogSys::~LogSys()
{
	//释放临界区  
	DeleteCriticalSection(&m_cs);
	//关闭文件流  
	if (m_pFileStream)
	{
		fclose(m_pFileStream);
		m_pFileStream = NULL;
	}
}

//文件全路径得到文件名  
const char *LogSys::path_file(const char *path, char splitter)
{
	return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
}

//写严重错误信息 奔溃信息
void LogSys::Fatal(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别  
	if (LogLevel::LogLevel_Fatal > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度  
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数  
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

	//写日志文件  
	Trace(strLog);
}

//写错误信息  执行失败信息
void LogSys::Error(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别  
	if (LogLevel::LogLevel_Error > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度  
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
    if (strResult.empty()) {
		return;
	}
	string strLog = strErrorPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;font-size:18px;color:#ff5151\">" + strResult + "</div>\n");

	//写日志文件  
	Trace(strLog);
}

//写警告信息  理想预期之外的信息
void LogSys::Warning(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别  
	if (LogLevel::LogLevel_Warning > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度  
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数  
	}
#ifndef __RELEASE__
    cout << strResult;
#endif
    if (strResult.empty()) {
		return;
	}
	string strLog = strWarningPrefix;
    strLog.append(GetTime()).append("<div style=\"font-weight:bold;color:#ff8f00\">" + strResult + "</div>\n");

	//写日志文件  
	Trace(strLog);
}


//写一般信息 提示信息
void LogSys::Info(const char *lpcszFormat, ...)
{
    //判断当前的写日志级别 mark fixme 当前级别打印但不记录
    //if (LogLevel::LogLevel_Info > m_nLogLevel)
    //	return;
	string strResult;
	if (NULL != lpcszFormat) {
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度  
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数  
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

	//写日志文件  
	Trace(strLog);
#endif
}

//获取系统当前时间  
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

//改变写日志级别  
void LogSys::ChangeLogLevel(LogLevel nLevel)
{
	m_nLogLevel = nLevel;
}

//写文件操作  
void LogSys::Trace(const string &strLog)
{
	try {
		//进入临界区  
		EnterCriticalSection(&m_cs);
		//若文件流没有打开，则重新打开  
		if (NULL == m_pFileStream) {
			fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
			if (!m_pFileStream) {
				return;
			}
		}
        //写日志信息到文件流
        fprintf(m_pFileStream, "\n%s\n", strLog.c_str());
		fflush(m_pFileStream);
		//离开临界区  
		LeaveCriticalSection(&m_cs);
	}
	//若发生异常，则先离开临界区，防止死锁  
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
		va_start(marker, lpcszFormat); //初始化变量参数  
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度  
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0) {
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数  
	}
	return strResult;
}
