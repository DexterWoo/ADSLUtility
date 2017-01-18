#pragma once

#include "gtl/str.h"
#include "SmartPtr.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <time.h>
#include <io.h>
#include <mutex>
#include <locale>
#include <tchar.h>
#include <time.h>
#include <fcntl.h>

#include <Shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

enum LOG_LEVEL
{
	LOG_LEVEL_TRACE = 0,
	LOG_LEVEL_DEBUG	= 1,
	LOG_LEVEL_INFO	= 2,
	LOG_LEVEL_WARN	= 3,
	LOG_LEVEL_ERROR	= 4,
	LOG_LEVEL_FATAL	= 5,
	LOG_LEVEL_NONE	= 6
};

#define LOG_LINE_MAX_BYTES (16 * 1024)

class ILogger
{
public:
	virtual void print(LOG_LEVEL level, const char* log) = 0;
	virtual void print(LOG_LEVEL level, const wchar_t* log) = 0;

	virtual int type() = 0;
};

class FileLogger : public ILogger
{
public:
	FileLogger(const char* filename, LOG_LEVEL level = LOG_LEVEL_INFO, unsigned int maxFilesize = 3 * 1024 * 1024)
	{
		_tsetlocale(LC_CTYPE, _T(""));

		m_file = nullptr;
		m_level = level;
		memset(m_filename, 0, sizeof(m_filename) / sizeof(m_filename[0]));

		if(filename == nullptr || strlen(filename) > 260)
			return;

		strcpy_s(m_filename, 260, filename);

		m_file = CreateFile(gtl::tstr_warp(m_filename), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(m_file == nullptr)
			return;

		DWORD highSize = 0;
		DWORD size = GetFileSize(m_file, &highSize);
		if(size == 0)
		{
			DWORD bytesWritten = 0;
			unsigned char flag[] = { 0xEF, 0xBB, 0xBF };
			WriteFile(m_file, flag, sizeof(flag), &bytesWritten, nullptr);
		}
		else if(size > maxFilesize)
		{
			reset();
		}

		SetFilePointer(m_file, 0, nullptr, FILE_END);
	}

	virtual ~FileLogger()
	{
		if(m_file != nullptr)
			CloseHandle(m_file);
	}

public:
	virtual void print(LOG_LEVEL level, const char* log)
	{
		if(level < m_level)
			return;

		if(m_file == nullptr)
			return;

		DWORD bytesWritten = 0;
		gtl::str lg(gtl::wstr(log), CP_UTF8);
		WriteFile(m_file, lg.c_str(), lg.length(), &bytesWritten, nullptr);
		FlushFileBuffers(m_file);
	}

	virtual void print(LOG_LEVEL level, const wchar_t* log)
	{
		if(level < m_level)
			return;

		if(m_file == nullptr)
			return;

		DWORD bytesWritten = 0;
		gtl::str lg(log, CP_UTF8);
		WriteFile(m_file, lg.c_str(), lg.length(), &bytesWritten, nullptr);
		FlushFileBuffers(m_file);
	}

	virtual int type()
	{
		return 'fllg';
	}

protected:
	void reset()
	{
		if(m_file == nullptr)
			return;

		CloseHandle(m_file);

		m_file = CreateFile(gtl::tstr_warp(m_filename), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(m_file != nullptr)
		{
			DWORD bytesWritten = 0;
			unsigned char flag[] = { 0xEF, 0xBB, 0xBF };
			WriteFile(m_file, flag, sizeof(flag), &bytesWritten, nullptr);
		}
	}

public:
	HANDLE m_file;
	LOG_LEVEL m_level;
	char m_filename[260];
};

class DebugLogger : public ILogger
{
public:
	DebugLogger(LOG_LEVEL level = LOG_LEVEL_DEBUG)
	{
		m_level = level;
	}

public:
	virtual void print(LOG_LEVEL level, const char* log)
	{
#ifndef _DEBUG
		if(level > m_level)
			return;
#endif

		OutputDebugStringA(log);
	}

	virtual void print(LOG_LEVEL level, const wchar_t* log)
	{
#ifndef _DEBUG
		if(level > m_level)
			return;
#endif

		OutputDebugStringW(log);
	}

	virtual int type()
	{
		return 'dblg';
	}

protected:
	LOG_LEVEL m_level;
};

class Logger
{
public:
	static Logger& Instance()
	{
		static Logger log;
		return log;
	}

public:
	void InitByFileAndDebugLogger(const char* folder, const char* subfolder, const char* filename)
	{
		char logpath[512] = {0};
		GetTempPathA(sizeof(logpath) / sizeof(logpath[0]), logpath);
		PathAppendA(logpath, folder);
		CreateDirectoryA(logpath, NULL);

		PathAppendA(logpath, subfolder);
		CreateDirectoryA(logpath, NULL);

		PathAppendA(logpath, filename);

		add(new DebugLogger());
		add(new FileLogger(logpath));
	}

public:
	template<class Char>
	void trace(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_TRACE, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void debug(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_DEBUG, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void info(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_INFO, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void warn(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_WARN, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void error(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_ERROR, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void fatal(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_FATAL, tag, format, args);
		va_end(args);
	}

public:
	template<class Char>
	void t(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_TRACE, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void d(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_DEBUG, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void i(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_INFO, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void w(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_WARN, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void e(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_ERROR, tag, format, args);
		va_end(args);
	}

	template<class Char>
	void f(const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(LOG_LEVEL_FATAL, tag, format, args);
		va_end(args);
	}

public:
	template<class Char>
	void output(LOG_LEVEL level, const Char* tag, const Char* format, ...)
	{
		va_list args;
		va_start(args, format);
		output(level, tag, format, args);
		va_end(args);
	}

public:
	void add(ILogger* logger)
	{
		if(logger == nullptr)
			return;

		for(auto& lgr : m_loggers)
		{
			if(lgr->type() == logger->type())
				return;
		}

		m_loggers.push_back(std::shared_ptr<ILogger>(logger));
	}

	void remove(ILogger* logger)
	{
		for(auto& lgr : m_loggers)
		{
			if(lgr.get() == logger)
			{
				m_loggers.erase(std::remove(m_loggers.begin(), m_loggers.end(), lgr), m_loggers.end());
				break;
			}
		}
	}

protected:
	void output(LOG_LEVEL level, const char* tag, const char* format, va_list args)
	{
		time_t t = 0;
		time(&t);
		struct tm nowptr;
		struct tm* now = &nowptr;
		localtime_s(&nowptr, &t);

		long pId = 0;
		long threadId = 0;
#ifdef _WIN32
		pId = ::GetCurrentProcessId();
		threadId = ::GetCurrentThreadId();
#endif

		char prefix[128] = {0};
		sprintf_s(prefix, 128, "%d-%02d-%02d %02d:%02d:%02d (P:%d|T:%d) %s ",
			now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, pId, threadId, tag);
		int len = strlen(prefix);
		
		CSmartArrayPtr<char> log;
		log = new char[LOG_LINE_MAX_BYTES + 4];
		ZeroMemory((char*)log, LOG_LINE_MAX_BYTES + 4);
		int count = vsnprintf_s(log + len, LOG_LINE_MAX_BYTES - len, _TRUNCATE, format, args);
		if (count <= 0)
			strcpy_s(log + LOG_LINE_MAX_BYTES - 1, 4, "...");

		memcpy(log, prefix, len * sizeof(char));

		len = strlen(log);
		if(len < LOG_LINE_MAX_BYTES + 4 - 1)
			log[len] = '\n';
		else
			log[LOG_LINE_MAX_BYTES + 4 - 1] = '\n';

		if(len + 1 < LOG_LINE_MAX_BYTES + 4 - 1)
			log[len + 1] = 0;

		for(auto& lgr : m_loggers)
		{
			std::unique_lock<std::mutex> locker(m_mutex);
			lgr->print(level,  log);
		}
	}

	void output(LOG_LEVEL level, const wchar_t* tag, const wchar_t* format, va_list args)
	{
		time_t t = 0;
		time(&t);
		struct tm nowptr;
		struct tm* now = &nowptr;
		localtime_s(&nowptr, &t);

		long pId = 0;
		long threadId = 0;
#ifdef _WIN32
		pId = ::GetCurrentProcessId();
		threadId = ::GetCurrentThreadId();
#endif

		wchar_t prefix[128] = {0};
		wsprintf(prefix, L"%d-%02d-%02d %02d:%02d:%02d (P:%d|T:%d) %s ", 
			now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, pId, threadId, tag);
		int len = wcslen(prefix);

		CSmartArrayPtr<wchar_t> log;
		log = new wchar_t[LOG_LINE_MAX_BYTES + 4];
		ZeroMemory((wchar_t*)log, LOG_LINE_MAX_BYTES + 4);
		int count = _vsnwprintf_s(log + len, LOG_LINE_MAX_BYTES - len, _TRUNCATE, format, args);
		if (count <= 0)
			wcscpy_s(log + LOG_LINE_MAX_BYTES - 1, 4, L"...");

		memcpy(log, prefix, len * sizeof(wchar_t));

		len = wcslen(log);
		if(len < LOG_LINE_MAX_BYTES + 4 - 1)
			log[len] = '\n';
		else
			log[LOG_LINE_MAX_BYTES + 4 - 1] = '\n';

		if(len + 1 < LOG_LINE_MAX_BYTES + 4 - 1)
			log[len + 1] = 0;

		for(auto& lgr : m_loggers)
		{
			std::unique_lock<std::mutex> locker(m_mutex);
			lgr->print(level, log);
		}
	}

protected:
	std::mutex m_mutex;
	std::vector<std::shared_ptr<ILogger>> m_loggers;
};

#define Log Logger::Instance()