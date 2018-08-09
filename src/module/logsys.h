/*
定义:
LogSys Log(LogLevel_Info, LogSys::GetAppPathA().append("log\\"));
#define pr(fmt) do { \
    Log.Info("%s  %s  %d\n\t" fmt, __FUNCTION__, __LINE__); \
} while(0);

in the  function:
	pr("I'm a log trace");

show:
Info	2018-05-29 14:18:22
c:\users\administrator\desktop\svn\irexam\src\ui\irexam.cpp  IrExam::IrExam  41
	i'm a log trace
tips:
#define __RELEASE__		 //用于关闭打印消息
*/

#ifndef _LOGSYS_H_
#define _LOGSYS_H_
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <QString>

/*****************************************************************************
	Copyright	: Yaqian Group
	Author		: Mark_Huang ( hacker.do@163.com )
	Date		: 2018.05.29
	Description	: 用于快捷打印调试位置 & 宏 (与GNU C语法有点不同)
 *****************************************************************************/
//写严重错误信息 奔溃信息
#define PrFatal(fmt, ...) do { \
    LogSys::Get()->Fatal("%s %3d\t" fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

//写错误信息  执行失败信息 测试不通过信息
#define PrError(fmt, ...) do { \
    LogSys::Get()->Error("%s %3d\t" fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

//写警告信息  理想预期之外的信息
#define PrWarning(fmt, ...) do { \
    LogSys::Get()->Warning("%s %3d\t" fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);

//写一般信息 提示信息
    //处理中文编码等问题 (使用 utf-8 bom 编码)
#define PrInfo Pr
#define Pr(fmt, ...) do { \
    LogSys::Get()->Info("%s %3d\t" fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__); \
} while(0);


//日志级别的提示信息  
static const std::string strFatalPrefix = "Fatal\t";
static const std::string strErrorPrefix = "Error\t";
static const std::string strWarningPrefix = "Warning\t";
static const std::string strInfoPrefix = "Info\t";

//日志级别枚举  
typedef enum {
	LogLevel_Stop = 0,  //什么都不记录  
	LogLevel_Fatal,     //只记录严重错误  
	LogLevel_Error,     //记录严重错误，普通错误  
	LogLevel_Warning,   //记录严重错误，普通错误，警告  
	LogLevel_Info       //记录严重错误，普通错误，警告，提示信息(也就是全部记录)  
} LogLevel;

class LogSys {
public:
	static LogSys * Get();
	//析构函数
	virtual ~LogSys();
public:
	//写严重错误信息  
	void Fatal(const char *lpcszFormat, ...);
	//写错误信息  
	void Error(const char *lpcszFormat, ...);
	//写警告信息 
	void Warning(const char *lpcszFormat, ...);
	//写提示信息  
    void Info(QString qtString, ...);
    void Info(const char *lpcszFormat, ...);
	//改变写日志级别  
	void ChangeLogLevel(LogLevel nLevel);
	//获取程序运行路径  
	static std::string GetAppPathA();
	//格式化字符串  
	static std::string FormatString(const char *lpcszFormat, ...);
private:
	//写文件操作  
	void Trace(const std::string &strLog);
	//获取当前系统时间  
	std::string GetTime();
	//文件全路径得到文件名  
	const char *path_file(const char *path, char splitter);
protected:
	//nLogLevel：日志记录的等级，可空  
	//strLogPath：日志目录，可空  
	//strLogName：日志名称，可空  
	LogSys(LogLevel nLogLevel = LogLevel::LogLevel_Info, const std::string strLogPath = "", const std::string strLogName = "");
	//写日志文件流  
	FILE * m_pFileStream;
	//写日志级别  
	LogLevel m_nLogLevel;
	//日志目录  
	std::string m_strLogPath;
	//日志的名称
	std::string m_strLogName;
	//日志文件全路径  
	std::string m_strLogFilePath;
	//线程同步的临界区变量  
	CRITICAL_SECTION m_cs;
};

#endif //_LOGSYS_
