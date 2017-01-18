#pragma once

#include <tchar.h>
#include <string>
#include <stdarg.h>
#include "inner.h"

#pragma warning(push)
#pragma warning(disable: 4996)

namespace gtl
{
	
#ifdef _UNICODE
	typedef std::wstring	tstring;
#else
	typedef std::string		tstring;
#endif

namespace string
{

inline char* tcscpy(char* str1, const char* str2) {
	return strcpy(str1, str2);
}

inline wchar_t* tcscpy(wchar* wcs1, const wchar* wcs2) {
	return wcscpy(wcs1, wcs2);
}

inline char* tcsncpy(char* str1, const char* str2, size_t count) {
	return strncpy(str1, str2, count);
}

inline wchar_t* tcsncpy(wchar* wcs1, const wchar* wcs2, size_t count) {
	return wcsncpy(wcs1, wcs2, count);
}

inline char* tcscat(char* str1, const char* str2) {
	return strcat(str1, str2);
}

inline wchar_t* tcscat(wchar* wcs1, const wchar* wcs2) {
	return wcscat(wcs1, wcs2);
}

inline char* tcsncat(char* str1, const char* str2, size_t count) {
	return strncat(str1, str2, count);
}

inline wchar_t* tcsncat(wchar* wcs1, const wchar* wcs2, size_t count) {
	return wcsncat(wcs1, wcs2, count);
}

inline int tcscmp(const char* src, const char* dst) {
	return strcmp(src, dst);
}

inline int tcscmp(const wchar* src, const wchar* dst) {
	return wcscmp(src, dst);
}

inline int tcsncmp(const char* src, const char* dst, size_t count) {
	return strncmp(src, dst, count);
}

inline int tcsncmp(const wchar* src, const wchar* dst, size_t count) {
	return wcsncmp(src, dst, count);
}

inline int tcsicmp(const char* src, const char* dst) {
	return stricmp(src, dst);
}

inline int tcsicmp(const wchar* src, const wchar* dst) {
	return wcsicmp(src, dst);
}

inline int tcsnicmp(const char* src, const char* dst, size_t count) {
	return strnicmp(src, dst, count);
}

inline int tcsnicmp(const wchar* src, const wchar* dst, size_t count) {
	return wcsnicmp(src, dst, count);
}

inline size_t tcslen(const char* str) {
	return strlen(str);
}

inline size_t tcslen(const wchar* str) {
	return wcslen(str);
}

inline char* tcschr(char* str1, char ch) {
	return strchr(str1, (int)(unsigned char)ch);
}

inline wchar_t* tcschr(wchar* wcs1, wchar_t ch) {
	return wcschr(wcs1, ch);
}

inline char* tcsrchr(char* str1, char ch) {
	return strrchr(str1, (int)(unsigned char)ch);
}

inline wchar_t* tcsrchr(wchar* wcs1, wchar_t ch) {
	return wcsrchr(wcs1, ch);
}

inline char* tcsstr(char* str1, const char* str2) {
	return strstr(str1, str2);
}

inline wchar_t* tcsstr(wchar* wcs1, const wchar* wcs2) {
	return wcsstr(wcs1, wcs2);
}

inline char* tcsrev(char* str) {
	return strrev(str);
}

inline wchar* tcsrev(wchar* str) {
	return wcsrev(str);
}

inline char* tcslwr(char* str) {
	return strlwr(str);
}

inline wchar* tcslwr(wchar* str) {
	return wcslwr(str);
}

inline char* tcsupr(char* str) {
	return strupr(str);
}

inline wchar* tcsupr(wchar* str) {
	return wcsupr(str);
}

inline void tsprintf(char* dst, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt); 
	vsprintf(dst, fmt, args);
	va_end(args);
}

inline void tsprintf(wchar* dst, const wchar* fmt, ...) {
	va_list args;
	va_start(args, fmt); 
	////////wvsprintf(dst, fmt, args);
	va_end(args);
}

} // end namespace string

} // end namespace gtl

#pragma warning(pop)