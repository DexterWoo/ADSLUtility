#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4146)
#pragma warning(disable: 4290)

#include <string>
#include <vector>
#include "tchr.h"

namespace gtl
{

template<class TChar> class str_t;
typedef str_t<char>			str;
typedef str_t<wchar>		wstr;

template<class TChar> class	str_warp_t;
typedef str_warp_t<char>	str_warp;
typedef str_warp_t<wchar>	wstr_warp;

#ifdef _UNICODE
typedef wstr		tstr;
typedef wstr_warp	tstr_warp;
#else
typedef str			tstr;
typedef str_warp	tstr_warp;
#endif // _UNICODE

template<class TChar>
class str_t
{
protected:
	template<class T> struct traits;
	template<> struct traits<wchar> {
		typedef char trans_type;
		typedef wchar unsigned_type;
	};
	template<> struct traits<char> {
		typedef wchar trans_type;
		typedef unsigned char unsigned_type;
	};
	template<> struct traits<unsigned char> {
		typedef wchar trans_type;
		typedef unsigned char unsigned_type;
	};

public:
	typedef TChar value_type;
	typedef typename traits<TChar>::trans_type trans_type;
	typedef typename traits<TChar>::unsigned_type unsigned_type;

public:
	str_t();
	template<class InnerType> explicit str_t(InnerType);
	explicit str_t(float, int);
	explicit str_t(double, int);

	// 这组构造函数有第二参数,是为了使用下面一组构造函数时
	// unicode和multibyte都能编译过,在实现时参数没有意义.
	str_t(TChar*, unsigned int = 0);
	str_t(const TChar*, unsigned int = 0);
	str_t(const str_t&, unsigned int = 0);
	str_t(const std::basic_string<TChar>&, unsigned int = 0);

#ifdef _WINDOWS_
	explicit str_t(trans_type*, unsigned int /*cp*/ = CP_ACP);
	explicit str_t(const trans_type*, unsigned int = CP_ACP);
	explicit str_t(const str_t<trans_type>&, unsigned int = CP_ACP);
	explicit str_t(const std::basic_string<trans_type>&, unsigned int = CP_ACP);
#endif // _WINDOWS_

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	str_t(str_t&& val);
	str_t& operator=(str_t&& val);
	str_t(str_warp_t<TChar>&& val);
	str_t& operator=(str_warp_t<TChar>&& val);
#endif // defined(_MSC_VER) && (_MSC_VER >= 1600)

	virtual ~str_t();

public:
	str_t operator+(TChar) const;
	str_t operator+(const TChar*) const;
	str_t operator+(const str_t&) const;
	str_t operator+(const std::basic_string<TChar>&) const;

	str_t& operator=(TChar*);
	str_t& operator=(const TChar*);
	str_t& operator=(const str_t&);
	str_t& operator=(const std::basic_string<TChar>&);

	str_t& operator+=(TChar*);
	str_t& operator+=(const TChar*);
	str_t& operator+=(const str_t&);
	str_t& operator+=(const std::basic_string<TChar>&);
	template<class InnerType> str_t& operator+=(InnerType);
#ifdef _WINDOWS_
	str_t& operator+=(const trans_type*);
	str_t& operator+=(const str_t<trans_type>&);
	str_t& operator+=(const std::basic_string<trans_type>&);
#endif // _WINDOWS_

	str_t& operator<<(TChar*);
	str_t& operator<<(const TChar*);
	str_t& operator<<(const str_t&);
	str_t& operator<<(const std::basic_string<TChar>&);
	template<class InnerType> str_t& operator<<(InnerType);
	template<class InnerType> str_t& operator>>(InnerType&) const;
#ifdef _WINDOWS_
	str_t& operator<<(const trans_type*);
	str_t& operator<<(const str_t<trans_type>&);
	str_t& operator<<(const std::basic_string<trans_type>&);
#endif // _WINDOWS_

public:
	bool operator==(const TChar*) const;
	bool operator==(const str_t&) const;
	bool operator==(const std::basic_string<TChar>&) const;

	bool operator!=(const TChar*) const;
	bool operator!=(const str_t&) const;
	bool operator!=(const std::basic_string<TChar>&) const;

	bool operator>(const TChar*) const;
	bool operator>(const str_t&) const;
	bool operator>(const std::basic_string<TChar>&) const;

	bool operator<(const TChar*) const;
	bool operator<(const str_t&) const;
	bool operator<(const std::basic_string<TChar>&) const;

	bool operator>=(const TChar*) const;
	bool operator>=(const str_t&) const;
	bool operator>=(const std::basic_string<TChar>&) const;

	bool operator<=(const TChar*) const;
	bool operator<=(const str_t&) const;
	bool operator<=(const std::basic_string<TChar>&) const;

public:
	operator const TChar*() const throw();

	const TChar* c_str() const throw();

	const TChar* data() const throw();
	TChar*		 data() throw();

	TChar&		 operator[](int) throw(const char*);
	const TChar& operator[](int) const throw(const char*);

	TChar&		 front() throw(const char*);
	const TChar& front() const throw(const char*);

	TChar&		 back() throw(const char*);
	const TChar& back() const throw(const char*);

public:
	// 将字符串转换成内置类型,比如int,float之类的,如果转换成其它类型则编译出错
	template<class InnerType> InnerType cast() const;
	template<> bool cast() const
	{
		if(m_value == NULL)
			return false;

		if(icmp(str_t("true")))
			return true;
		else if(icmp(str_t("false")))
			return false;

		return cast<int>() != 0;
	}

#ifdef _WINDOWS_
	// 如果当前对象是str对象则转换成wstr对象,如果当前对象是wstr对象则转换成str对象
	// 如果str对象要转换成utf8格式则应该调用utf8()函数
	str_t<trans_type>		 trans(unsigned int /*codepage*/ = CP_ACP) const;
	const str_t<trans_type>& trans(str_t<trans_type>&, unsigned int /*codepage*/ = CP_ACP) const;

	// 转换成utf8编码
	str_t<char> utf8() const;
#endif // _WINDOWS_

	str_t& ltrim();
	str_t& rtrim();
	str_t& trim();
	str_t& ltrim(const TChar*);
	str_t& rtrim(const TChar*);
	str_t& trim(const TChar*);
	str_t& ltrim(const std::basic_string<TChar>&);
	str_t& rtrim(const std::basic_string<TChar>&);
	str_t& trim(const std::basic_string<TChar>&);
	str_t& lower();
	str_t& upper();
	str_t& reverse();
	str_t& replace(TChar /*cold*/, TChar /*cnew*/);
	str_t& replace(const TChar* /*strold*/, const TChar* /*strnew*/);
	str_t& replace(const std::basic_string<TChar>& /*strold*/, const std::basic_string<TChar>& /*strnew*/);
	str_t& replace(const std::basic_string<TChar>& /*strold*/, const TChar* /*strnew*/);
	str_t& replace(const TChar* /*strold*/, const std::basic_string<TChar>& /*strnew*/);

	// 如果last等于0则表示到字符串的最后,如果参数为负数,则从后面算起.
	str_t substr(int /*first*/, int /*last*/ = 0) const;

	void clear();
	bool empty() const;
	TChar* resize(size_t);
	size_t size() const;
	size_t length() const;
	size_t capacity() const;
	unsigned int hash() const;
	unsigned long color() const;

	str_t& cpy(const TChar* /*data*/, size_t /*size*/);
	str_t& cat(const TChar* /*data*/, size_t /*size*/);
	str_t& mcpy(size_t /*index*/, const char* /*data*/, size_t /*size*/);

	// 判断是否相等,区分大写小
	bool cmp(const TChar*, size_t /*count*/ = 0) const;
	bool cmp(const str_t&, size_t /*count*/ = 0) const;
	bool cmp(const std::basic_string<TChar>&, size_t /*count*/ = 0) const;

	// 判断是否相等,不区分大小写
	bool icmp(const TChar*, size_t /*count*/ = 0) const;
	bool icmp(const str_t&, size_t /*count*/ = 0) const;
	bool icmp(const std::basic_string<TChar>&, size_t /*count*/ = 0) const;

	size_t find(TChar, size_t /*start*/ = 0) const;
	size_t find(const TChar*, size_t /*start*/ = 0) const;
	size_t find(const str_t&, size_t /*start*/ = 0) const;
	size_t find(const std::basic_string<TChar>&, size_t /*start*/ = 0) const;

	// 查找多个分隔符在字符串中第一次出现的位置,如果查找成功则delimiter指向第一个出现的分隔符
	size_t find(const TChar** /*delimiters*/, int /*count*/, const TChar** /*delimiter*/ = NULL, size_t /*start*/ = 0) const;

	template<class TStr, template<class, class> class Container>
	size_t find(const Container<TStr, std::allocator<TStr> >& /*delimiters*/, const TChar** /*delimiter*/ = NULL, size_t /*start*/ = 0) const;

	size_t rfind(TChar) const;
	size_t rfind(const TChar*) const;
	size_t rfind(const str_t&) const;
	size_t rfind(const std::basic_string<TChar>&) const;

	// 分割字符串
	template<class TStr, template<class, class> class Container>
	Container<TStr, std::allocator<TStr> >& split(Container<TStr, std::allocator<TStr> >& /*sub_strs*/, const TChar* /*delimiter*/) const;

	// 分割字符串,但只取前面count个结果
	template<class TStr, template<class, class> class Container>
	Container<TStr, std::allocator<TStr> >& split(Container<TStr, std::allocator<TStr> >& /*sub_strs*/, const TChar* /*delimiter*/, int /*count*/) const;

	// 用多个分隔符分割字符串
	template<class TStr, template<class, class> class Container>
	Container<TStr, std::allocator<TStr> >& split(Container<TStr, std::allocator<TStr> >& /*sub_strs*/, const TChar** /*delimiters*/, int /*count*/) const;

	// 获取以delimiter分割的第index个子字符串
	str_t split(const TChar* /*delimiter*/, int /*index*/ = 0) const;

	// 将当前字符串与指定字符串数组连接成一个字符串
	template<class TStr, template<class, class> class Container>
	str_t join(Container<TStr, std::allocator<TStr> >& /*strs*/) const;

	// 将当前字符串与指定字符串数组连接成一个字符串
	template<class TStr, template<class, class> class Container>
	str_t join(Container<TStr, std::allocator<TStr> >& /*strs*/, size_t /*index*/, size_t /*count*/) const;

	// 将当前字符串与指定字符串数组连接成一个字符串
	str_t join(const TChar** /*strs*/, size_t /*index*/, size_t /*count*/) const;

protected:
	TChar* find(const TChar* /*str*/, const TChar** /*delimiters*/, int /*count*/, const TChar** /*delimiter*/) const;

protected:
	TChar* m_value;
	size_t m_length;
};

template<class TChar>
class str_warp_t : public str_t<TChar>
{
	template<class>
	friend class str_t;

public:
	str_warp_t(TChar* val, unsigned int = 0)
	{
		m_value = val;
		m_length = length();
		m_new_string = false;
	}

	str_warp_t(const TChar* val, unsigned int = 0)
	{
		m_value = const_cast<TChar*>(val);
		m_length = length();
		m_new_string = false;
	}

	str_warp_t(const std::basic_string<TChar>& val, unsigned int = 0)
	{
		m_value = const_cast<TChar*>(val.c_str());
		m_length = length();
		m_new_string = false;
	}

#ifdef _WINDOWS_
	explicit str_warp_t(trans_type* val, unsigned int cp = CP_ACP)
		: str_t(val, cp)
	{
		m_new_string = true;
	}

	explicit str_warp_t(const trans_type* val, unsigned int cp = CP_ACP)
		: str_t(val, cp)
	{
		m_new_string = true;
	}

	explicit str_warp_t(const str_t<trans_type>& val, unsigned int cp = CP_ACP)
		: str_t(val, cp)
	{
		m_new_string = true;
	}

	explicit str_warp_t(const std::basic_string<trans_type>& val, unsigned int cp = CP_ACP)
		: str_t(val, cp)
	{
		m_new_string = true;
	}
#endif // _WINDOWS_

	virtual ~str_warp_t()
	{
		clear();
	}

public:
	str_warp_t& operator=(TChar* val)
	{
		if(m_value == val)
			return *this;

		clear();

		m_value = val;
		m_length = length();
		m_new_string = false;

		return *this;
	}

	str_warp_t& operator=(const TChar* val)
	{
		if(m_value == val)
			return *this;

		clear();

		m_value = const_cast<TChar*>(val);
		m_length = length();
		m_new_string = false;

		return *this;
	}

	str_warp_t& operator=(const str_warp_t& val)
	{
		if(this == &val)
			return *this;

		clear();

		m_value = const_cast<TChar*>(val.c_str());
		m_length = length();
		m_new_string = false;

		return *this;
	}

	str_warp_t& operator=(const std::basic_string<TChar>& val)
	{
		if(m_value == val.c_str())
			return *this;

		clear();

		m_value = const_cast<TChar*>(val.c_str());
		m_length = length();
		m_new_string = false;

		return *this;
	}

	void clear()
	{
		if(m_new_string && m_value != NULL)
			free(m_value);

		m_value = NULL;
		m_length = 0;
	}

public:
	const TChar& front() const throw(const char*)
	{
		return str_t<TChar>::front();
	}

	const TChar& back() const throw(const char*)
	{
		return str_t<TChar>::back();
	}

private:
	operator str_t<TChar>&();
	operator const str_t<TChar>&();

	str_warp_t& operator+=(TChar*);
	str_warp_t& operator+=(const TChar*);
	str_warp_t& operator+=(const str_warp_t&);
	str_warp_t& operator+=(const std::basic_string<TChar>&);

	str_warp_t& operator<<(TChar*);
	str_warp_t& operator<<(const TChar*);
	str_warp_t& operator<<(const str_warp_t&);
	str_warp_t& operator<<(const std::basic_string<TChar>&);

	str_t& ltrim();
	str_t& rtrim();
	str_t& trim();
	str_t& ltrim(const TChar*);
	str_t& rtrim(const TChar*);
	str_t& trim(const TChar*);
	str_t& ltrim(const std::basic_string<TChar>&);
	str_t& rtrim(const std::basic_string<TChar>&);
	str_t& trim(const std::basic_string<TChar>&);
	str_t& lower();
	str_t& upper();
	str_t& reverse();
	str_t& replace(TChar /*cold*/, TChar /*cnew*/);
	str_t& replace(const TChar* /*strold*/, const TChar* /*strnew*/);
	str_t& replace(const std::basic_string<TChar>& /*strold*/, const std::basic_string<TChar>& /*strnew*/);
	str_t& replace(const std::basic_string<TChar>& /*strold*/, const TChar* /*strnew*/);
	str_t& replace(const TChar* /*strold*/, const std::basic_string<TChar>& /*strnew*/);

	str_t& cpy(const TChar*, size_t);
	str_t& cat(const TChar*, size_t);
	str_t& mcpy(size_t, const char*, size_t);

	TChar& operator[](int) throw(const char*);
	TChar& front() throw(const char*);
	TChar& back() throw(const char*);

protected:
	bool m_new_string;
};

//////////////////////////////////////////////////////////////////////////
namespace
{

struct local_cast
{
	static inline void cast(char sz[], bool val)					{ sprintf(sz, "%s", val ? "true" : "false"); }
	static inline void cast(char sz[], char val)					{ sprintf(sz, "%c", val); }
	static inline void cast(char sz[], short val)					{ sprintf(sz, "%d", val); }
	static inline void cast(char sz[], int val)						{ sprintf(sz, "%ld", val); }
	static inline void cast(char sz[], long val)					{ sprintf(sz, "%ld", val); }
	static inline void cast(char sz[], __int64 val)					{ sprintf(sz, "%lld", val); }
	static inline void cast(char sz[], unsigned char val)			{ sprintf(sz, "%c", val); }
	static inline void cast(char sz[], unsigned short val)			{ sprintf(sz, "%d", val); }
	static inline void cast(char sz[], unsigned int val)			{ sprintf(sz, "%lu", val); }
	static inline void cast(char sz[], unsigned long val)			{ sprintf(sz, "%u", val); }
	static inline void cast(char sz[], unsigned __int64 val)		{ sprintf(sz, "%llu", val); }
	static inline void cast(char sz[], float val)					{ sprintf(sz, "%f", val); }
	static inline void cast(char sz[], double val)					{ sprintf(sz, "%lf", val); }
	static inline void cast(char sz[], float val, int precision)	{ sprintf(sz, "%.*f", precision, val); }
	static inline void cast(char sz[], double val, int precision)	{ sprintf(sz, "%.*lf", precision, val); }

	static inline void cast(wchar sz[], bool val)					{ _swprintf(sz, L"%s", val ? L"true" : L"false"); }
	static inline void cast(wchar sz[], wchar val)					{ _swprintf(sz, L"%c", val); }
	static inline void cast(wchar sz[], short val)					{ _swprintf(sz, L"%d", val); }
	static inline void cast(wchar sz[], int val)					{ _swprintf(sz, L"%ld", val); }
	static inline void cast(wchar sz[], long val)					{ _swprintf(sz, L"%ld", val); }
	static inline void cast(wchar sz[], __int64 val)				{ _swprintf(sz, L"%lld", val); }
	static inline void cast(wchar sz[], unsigned short val)			{ _swprintf(sz, L"%d", val); }
	static inline void cast(wchar sz[], unsigned int val)			{ _swprintf(sz, L"%lu", val); }
	static inline void cast(wchar sz[], unsigned long val)			{ _swprintf(sz, L"%lu", val); }
	static inline void cast(wchar sz[], unsigned __int64 val)		{ _swprintf(sz, L"%llu", val); }
	static inline void cast(wchar sz[], float val)					{ _swprintf(sz, L"%f", val); }
	static inline void cast(wchar sz[], double val)					{ _swprintf(sz, L"%lf", val); }
	static inline void cast(wchar sz[], float val, int precision)	{ _swprintf(sz, L"%.*f", precision, val); }
	static inline void cast(wchar sz[], double val, int precision)	{ _swprintf(sz, L"%.*lf", precision, val); }
};

} // end of namespace

//////////////////////////////////////////////////////////////////////////
// 实现
template<class TChar>
str_t<TChar>::str_t()
{
	m_value = NULL;
	m_length = 0;
}

template<class TChar>
template<class InnerType>
str_t<TChar>::str_t(InnerType val)
{
	m_value = NULL;
	m_length = 0;
	operator+=(val);
}

template<class TChar>
str_t<TChar>::str_t(float val, int precision)
{
	m_value = NULL;
	m_length = 0;

	TChar sz[64] = {0};
	local_cast::cast(sz, val, int(precision < 0 ? 0 : precision));
	if(resize(length() + gtl::string::tcslen(sz) + 1))
		gtl::string::tcscat(m_value, sz);
}

template<class TChar>
str_t<TChar>::str_t(double val, int precision)
{
	m_value = NULL;
	m_length = 0;

	TChar sz[64] = {0};
	local_cast::cast(sz, val, int(precision < 0 ? 0 : precision));
	if(resize(length() + gtl::string::tcslen(sz) + 1))
		gtl::string::tcscat(m_value, sz);
}

template<class TChar>
str_t<TChar>::str_t(TChar* val, unsigned int)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

template<class TChar>
str_t<TChar>::str_t(const TChar* val, unsigned int)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

template<class TChar>
str_t<TChar>::str_t(const str_t& val, unsigned int)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

template<class TChar>
str_t<TChar>::str_t(const std::basic_string<TChar>& val, unsigned int)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

#ifdef _WINDOWS_
template<class TChar>
str_t<TChar>::str_t(trans_type* val, unsigned int codepage)
{
	m_value = NULL;
	m_length = 0;
	str_t<trans_type>(val).trans(*this, codepage);
}

template<class TChar>
str_t<TChar>::str_t(const trans_type* val, unsigned int codepage)
{
	m_value = NULL;
	m_length = 0;
	str_t<trans_type>(val).trans(*this, codepage);
}

template<class TChar>
str_t<TChar>::str_t(const str_t<trans_type>& val, unsigned int codepage)
{
	m_value = NULL;
	m_length = 0;
	str_t<trans_type>(val).trans(*this, codepage);
}

template<class TChar>
str_t<TChar>::str_t(const std::basic_string<trans_type>& val, unsigned int codepage)
{
	m_value = NULL;
	m_length = 0;
	str_t<trans_type>(val).trans(*this, codepage);
}
#endif // _WINDOWS_

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
template<class TChar>
str_t<TChar>::str_t(str_t&& val)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator=(str_t&& val)
{
	clear();

	m_value = val.m_value;
	m_length = val.m_length;

	val.m_value = NULL;
	val.m_length = 0;

	return *this;
}

template<class TChar>
str_t<TChar>::str_t(str_warp_t<TChar>&& val)
{
	m_value = NULL;
	m_length = 0;
	operator=(val);
}

template<class TChar>
str_t<TChar>& str_t<TChar>::operator=(str_warp_t<TChar>&& val)
{
	clear();

	if(val.m_new_string)
	{
		m_value = val.m_value;
		m_length = val.m_length;

		val.m_value = NULL;
		val.m_length = 0;
	}
	else
	{
		m_value = NULL;
		m_length = 0;
		operator=(val.c_str());
	}

	return *this;
}

#endif // defined(_MSC_VER) && (_MSC_VER >= 1600)

template<class TChar>
str_t<TChar>::~str_t()
{
	clear();
}

template<class TChar>
inline void str_t<TChar>::clear()
{
	if(m_value != NULL)
	{
		free(m_value);
		m_value = NULL;
		m_length = 0;
	}
}

template<class TChar>
inline bool str_t<TChar>::empty() const
{
	return length() == 0;
}

template<class TChar>
inline size_t str_t<TChar>::length() const
{
	if(m_value == NULL)
		return 0;

	return gtl::string::tcslen(m_value);
}

template<class TChar>
inline size_t str_t<TChar>::size() const
{
	return length();
}

template<class TChar>
inline size_t str_t<TChar>::capacity() const
{
	return m_length;
}

template<class TChar>
inline TChar* str_t<TChar>::resize(size_t size)
{
	if(size == 0)
	{
		clear();
		return m_value;
	}

	TChar* value = (TChar*)realloc(m_value, size * sizeof(TChar));
	if(value == NULL)
		return NULL;

	m_value = value;
	if(m_length < size)
		memset(m_value + m_length, 0, (size - m_length) * sizeof(TChar));

	m_length = size;
	return m_value;
}

template<class TChar>
unsigned int str_t<TChar>::hash() const
{
	if(m_value == NULL)
		return 0;

	unsigned int nr = 1;
	unsigned int nr2 = 4;
	const TChar* key = m_value;
	while(*key != 0)
	{
		nr ^= (((nr & 63) + nr2) * ((unsigned int)(unsigned char)*key++)) + (nr << 8);
		nr2 += 3;
	}

	return nr;
}

template<class TChar>
unsigned long str_t<TChar>::color() const
{
	if(empty())
		return 0;

	auto hex2dec = [](TChar chr) -> unsigned char
	{
		if(chr >= '0' && chr <= '9')
			return unsigned char(chr - '0');
		else if(chr >= 'A' && chr <= 'F')
			return unsigned char(chr - 'A' + 10);
		else if(chr >= 'a' && chr <= 'f')
			return unsigned char(chr - 'a' + 10);

		return 0;
	};

	auto clr_gene = [](TChar ch, TChar cl) -> unsigned long
	{
		return (ch << 4) + cl;
	};

	if(m_value[0] == 'R' && m_value[1] == 'G' && m_value[2] == 'B' && m_value[3] == '(')
	{
		gtl::tstr clr = m_value + 4;

		TChar sz[2] = { ',' };
		std::vector<gtl::str_t<TChar>> clrs;
		clr.split(clrs, sz);
		if(clrs.size() != 3)
			return 0;

		unsigned char r = clrs[0].cast<unsigned char>();
		unsigned char g = clrs[1].cast<unsigned char>();
		unsigned char b = clrs[2].cast<unsigned char>();
		return ((unsigned long)((0xFF000000 | r | ((unsigned short)g) << 8)) | (((unsigned long)b) << 16));
	}

	if(m_value[0] != '#' || length() != 7)
		return 0;

	unsigned long clr = 0xFF000000;
	clr +=  clr_gene(hex2dec(m_value[1]), hex2dec(m_value[2]));
	clr += (clr_gene(hex2dec(m_value[3]), hex2dec(m_value[4])) << 8);
	clr += (clr_gene(hex2dec(m_value[5]), hex2dec(m_value[6])) << 16);

	return clr;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator=(TChar* val)
{
	return operator=((const TChar*)val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator=(const TChar* val)
{
	if(m_value == val)
		return *this;

	clear();
	if(val == NULL)
		return *this;

	resize(gtl::string::tcslen(val) + 1);
	gtl::string::tcscpy(m_value, val);
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator=(const str_t& val)
{
	if(this == &val)
		return *this;

	clear();
	if(val.empty())
		return *this;

	resize(val.length() + 1);
	gtl::string::tcscpy(m_value, val.m_value);
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator=(const std::basic_string<TChar>& val)
{
	if(m_value == val.c_str())
		return *this;

	clear();
	if(val.empty())
		return *this;

	resize(val.length() + 1);
	gtl::string::tcscpy(m_value, val.c_str());
	return *this;
}

template<class TChar>
inline str_t<TChar> str_t<TChar>::operator+(TChar val) const
{
	if(m_value == NULL)
		return str_t(val);

	str_t value(*this);
	return value += val;
}

template<class TChar>
inline str_t<TChar> str_t<TChar>::operator+(const TChar* val) const
{
	if(m_value == NULL)
		return str_t(val);

	str_t value(*this);
	return value += val;
}

template<class TChar>
inline str_t<TChar> str_t<TChar>::operator+(const str_t& val) const
{
	if(m_value == NULL)
		return str_t(val);

	str_t value(*this);
	return value += val;
}

template<class TChar>
inline str_t<TChar> str_t<TChar>::operator+(const std::basic_string<TChar>& val) const
{
	if(m_value == NULL)
		return str_t(val);

	str_t value(*this);
	return value += val;
}

template<class TChar>
inline str_t<TChar> operator+(TChar val, const str_t<TChar>& s)
{
	return str_t<TChar>(val) + s;
}

template<class TChar>
inline str_t<TChar> operator+(const TChar* val, const str_t<TChar>& s)
{
	return str_t<TChar>(val) + s;
}

template<class TChar>
inline str_t<TChar> operator+(const std::basic_string<TChar>& val, const str_t<TChar>& s)
{
	return str_t<TChar>(val.c_str()) + s;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(TChar* val)
{
	return operator+=((const TChar*)val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const TChar* val)
{
	if(val != NULL && resize(length() + gtl::string::tcslen(val) + 1))
		gtl::string::tcscat(m_value, val);

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const str_t& val)
{
	if(val.m_value != NULL && resize(length() + val.length() + 1))
		gtl::string::tcscat(m_value, val);

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const std::basic_string<TChar>& val)
{
	if(!val.empty() && resize(length() + val.length() + 1))
		gtl::string::tcscat(m_value, val.c_str());

	return *this;
}

template<class TChar>
template<class InnerType>
inline str_t<TChar>& str_t<TChar>::operator+=(InnerType val)
{
	TChar sz[64] = {0};
	local_cast::cast(sz, val);
	if(resize(length() + gtl::string::tcslen(sz) + 1))
		gtl::string::tcscat(m_value, sz);

	return *this;

	//TChar tmp = 0;
	//unsigned digval = 0;
	//TChar sz[65] = {0};
	//TChar* digit = sz;
	//const TChar* value = m_value;
	//if(val < 0)
	//{
	//	*digit++ = '-';
	//	val = -val;
	//}

	//TChar* fdigit = digit;
	//const int radix = 10;
	//do
	//{
	//	digval = (unsigned)(val % radix);
	//	val /= radix;

	//	if (digval > 9)
	//		*digit++ = TChar(digval - 10 + 'a');
	//	else
	//		*digit++ = TChar(digval + '0');
	//}while(val > 0);

	//*digit-- = 0;
	//do
	//{
	//	tmp = *digit;
	//	*digit = *fdigit;
	//	*fdigit = tmp;
	//	--digit;
	//	++fdigit;
	//}while (fdigit < digit);

	//gtl::string::tcscat(m_value, sz);
	//return *this;
}

#ifdef _WINDOWS_
template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const trans_type* val)
{
	return operator+=(str_t<TChar>(val));
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const str_t<trans_type>& val)
{
	return operator+=(str_t<TChar>(val));
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator+=(const std::basic_string<trans_type>& val)
{
	return operator+=(str_t<TChar>(val));
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const trans_type* val)
{
	return operator+=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const str_t<trans_type>& val)
{
	return operator+=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const std::basic_string<trans_type>& val)
{
	return operator+=(val);
}
#endif // _WINDOWS_

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(TChar* val)
{
	return operator+=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const TChar* val)
{
	return operator+=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const str_t& val)
{
	return operator+=(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::operator<<(const std::basic_string<TChar>& val)
{
	return operator+=(val);
}

template<class TChar>
template<class InnerType>
inline str_t<TChar>& str_t<TChar>::operator<<(InnerType val)
{
	return operator+=(val);
}

template<class TChar>
template<class InnerType>
inline str_t<TChar>& str_t<TChar>::operator>>(InnerType& val) const
{
	val = cast<InnerType>();
	return *this;
}

template<class TChar>
inline bool str_t<TChar>::operator==(const TChar* val) const
{
	if(val == NULL || m_value == NULL)
		return val == m_value;

	return gtl::string::tcscmp(m_value, val) == 0;
}

template<class TChar>
inline bool str_t<TChar>::operator==(const str_t& val) const
{
	if(this == &val)
		return true;

	return operator==(val.m_value);
}

template<class TChar>
inline bool str_t<TChar>::operator==(const std::basic_string<TChar>& val) const
{
	return operator==(val.c_str());
}

template<class TChar>
inline bool str_t<TChar>::operator!=(const TChar* val) const
{
	return !operator==(val);
}

template<class TChar>
inline bool str_t<TChar>::operator!=(const str_t& val) const
{
	return !operator==(val);
}

template<class TChar>
inline bool str_t<TChar>::operator!=(const std::basic_string<TChar>& val) const
{
	return !operator==(val);
}

template<class TChar>
inline bool str_t<TChar>::operator>(const TChar* val) const
{
	if(m_value == NULL || val == NULL)
		return m_value != NULL;

	return gtl::string::tcscmp(m_value, val) > 0;
}

template<class TChar>
inline bool str_t<TChar>::operator>(const str_t& val) const
{
	if(this == &val)
		return false;

	return operator>(val.m_value);
}

template<class TChar>
inline bool str_t<TChar>::operator>(const std::basic_string<TChar>& val) const
{
	return operator>(val.c_str());
}

template<class TChar>
inline bool str_t<TChar>::operator<(const TChar* val) const
{
	if(m_value == NULL || val == NULL)
		return m_value != NULL;

	return gtl::string::tcscmp(m_value, val) < 0;
}

template<class TChar>
inline bool str_t<TChar>::operator<(const str_t& val) const
{
	if(this == &val)
		return false;

	return operator<(val.m_value);
}

template<class TChar>
inline bool str_t<TChar>::operator<(const std::basic_string<TChar>& val) const
{
	return operator<(val.c_str());
}

template<class TChar>
inline bool str_t<TChar>::operator>=(const TChar* val) const
{
	if(m_value == NULL || val == NULL)
		return m_value != NULL;

	return gtl::string::tcscmp(m_value, val) >= 0;
}

template<class TChar>
inline bool str_t<TChar>::operator>=(const str_t& val) const
{
	if(this == &val)
		return true;

	return operator>=(val.m_value);
}

template<class TChar>
inline bool str_t<TChar>::operator>=(const std::basic_string<TChar>& val) const
{
	return operator>=(val.c_str());
}

template<class TChar>
inline bool str_t<TChar>::operator<=(const TChar* val) const
{
	if(m_value == NULL || val == NULL)
		return m_value != NULL;

	return gtl::string::tcscmp(m_value, val) <= 0;
}

template<class TChar>
inline bool str_t<TChar>::operator<=(const str_t& val) const
{
	if(this == &val)
		return true;

	return operator<=(val.m_value);
}

template<class TChar>
inline bool str_t<TChar>::operator<=(const std::basic_string<TChar>& val) const
{
	return operator<=(val.c_str());
}

template<class TChar>
inline str_t<TChar>::operator const TChar*() const throw()
{
	return m_value;
}

template<class TChar>
inline const TChar* str_t<TChar>::c_str() const throw()
{
	return m_value;
}

template<class TChar>
inline const TChar* str_t<TChar>::data() const throw()
{
	return m_value;
}

template<class TChar>
inline TChar* str_t<TChar>::data() throw()
{
	return m_value;
}

template<class TChar>
inline TChar& str_t<TChar>::operator[](int val) throw(const char*)
{
	if(length() < size_t(val))
		throw "out of bounds";

	return m_value[val];
}

template<class TChar>
inline const TChar& str_t<TChar>::operator[](int val) const throw(const char*)
{
	if(length() < size_t(val))
		throw "out of bounds";

	return m_value[val];
}

template<class TChar>
inline TChar& str_t<TChar>::front() throw(const char*)
{
	if(empty())
		throw "out of bounds";

	return m_value[0];
}

template<class TChar>
inline const TChar& str_t<TChar>::front() const throw(const char*)
{
	if(empty())
		throw "out of bounds";

	return m_value[0];
}

template<class TChar>
inline TChar& str_t<TChar>::back() throw(const char*)
{
	if(empty())
		throw "out of bounds";

	return m_value[length() - 1];
}

template<class TChar>
inline const TChar& str_t<TChar>::back() const throw(const char*)
{
	if(empty())
		throw "out of bounds";

	return m_value[length() - 1];
}

template<class TChar>
template<class InnerType>
InnerType str_t<TChar>::cast() const
{
	if(m_value == NULL)
		return 0;

	int c = 0;
	int sign = 0;
	InnerType total = 0;
	const TChar* value = m_value;

	while(((unsigned)*value) <= 255 && isspace((int)(unsigned_type)*value))
		++value;

	c = (int)(unsigned_type)*value++;        
	sign = c;
	if(c == '-' || c == '+')
		c = (int)(unsigned_type)*value++;

	while(((unsigned)c) <= 255 && isdigit(c))
	{
		total = InnerType(10) * total + (c - '0');
		c = (int)(unsigned_type)*value++;
	}

	if(c == '.')
	{
		double power = 0.1;
		c = (int)(unsigned_type)*value++;
		while(((unsigned)c) <= 255 && isdigit(c))
		{
			total = InnerType(total + InnerType(power * (c - '0')));
			c = (int)(unsigned_type)*value++;
			power *= 0.1;
		}
	}

	if(sign == '-')
		return -total;
	else
		return total;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::lower()
{
	if(m_value == NULL)
		return *this;

	gtl::string::tcslwr(m_value);
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::upper()
{
	if(m_value == NULL)
		return *this;

	gtl::string::tcsupr(m_value);
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::reverse()
{
	if(m_value == NULL)
		return *this;

	gtl::string::tcsrev(m_value);
	return *this;
}
template<class TChar>
str_t<TChar>& str_t<TChar>::replace(TChar cold, TChar cnew)
{
	if(empty() || cold == cnew)
		return *this;

	TChar* start = m_value;
	TChar* end = m_value + length();
	for(; start < end; ++start)
	{
		if(*start == cold)
			*start = cnew;
	}

	return *this;
}

template<class TChar>
str_t<TChar>& str_t<TChar>::replace(const TChar* strold, const TChar* strnew)
{
	// 算法基本思想: 使用CString里的算法
	using namespace gtl::string;
	if(empty() || strold == NULL || *strold == 0 || strnew == NULL || tcscmp(strold, strnew) == 0)
		return *this;

	size_t oldlen = tcslen(strold);
	size_t newlen = tcslen(strnew);

	int count = 0;
	size_t len = length();
	size_t size = len;
	TChar* target = NULL;
	TChar* start = m_value;
	TChar* end = m_value + size;
	while(start < end)
	{
		while((target = tcsstr(start, strold)) != NULL)
		{
			++count;
			start = target + oldlen;
		}

		start += tcslen(start) + 1;
	}

	if(count <= 0)
		return *this;

	size_t newsize = size + (newlen - oldlen) * count + 1;
	if(newsize > size)
		resize(newsize);

	start = m_value;
	end = m_value + size;
	while(start < end)
	{
		while((target = tcsstr(start, strold)) != NULL)
		{
			int balance = size - (target - m_value + oldlen);
			memcpy(target + newlen, target + oldlen, balance * sizeof(TChar));
			memcpy(target, strnew, newlen * sizeof(TChar));
			start = target + newlen;
			target[newlen + balance] = 0;
			size += newlen - oldlen;
		}

		start += tcslen(start) + 1;
	}

	m_value[newsize - 1] = 0;
	if(newsize < len)
		resize(newsize);

	// 下面的算法不高效,直接换掉.
	//// 算法基本思想:
	//// 先找出m_value字符串中有几个strold子串,再分配替换的空间,再复制各字符串

	//if(m_value == NULL || strold == NULL || strnew == NULL || empty() || tcscmp(strold, strnew) == 0)
	//	return *this;

	//TChar* cp = m_value;
	//TChar* s1 = NULL;
	//const TChar* s2 = NULL;
	//std::vector<int> vecStroldIndex;
	//for(; *cp; ++cp) // 此for里有问题,替换最后一个字符会出错.
	//{
	//	s1 = cp;
	//	s2 = strold;
	//	while(*s1 && *s2 && !(*s1-*s2))
	//		s1++, s2++;

	//	if(!*s2)
	//	{
	//		vecStroldIndex.push_back(int(cp - m_value));
	//		cp = s1;
	//	}
	//}

	//if(vecStroldIndex.empty())
	//	return *this;

	//size_t lenold = tcslen(strold);
	//size_t lennew = tcslen(strnew);
	//size_t len = 1 + length() + (int)vecStroldIndex.size() * ((int)lennew - (int)lenold);
	//m_length = len;
	//TChar* value = (TChar*)malloc(sizeof(TChar) * len);
	//memset(value, 0, sizeof(TChar) * len);

	//vecStroldIndex.push_back((int)length());

	//tcsncpy(value, m_value, vecStroldIndex.front());
	//len = vecStroldIndex.front();
	//for(size_t i = 0; i < vecStroldIndex.size() - 1; ++i)
	//{
	//	tcsncpy(value + len, strnew, lennew);
	//	len += lennew;
	//	tcsncpy(value + len, m_value + vecStroldIndex[i] + lenold, vecStroldIndex[i + 1] - vecStroldIndex[i] - lenold);
	//	len += vecStroldIndex[i + 1] - vecStroldIndex[i] - lenold;
	//}
	//*(value + len) = 0;
	//
	//free(m_value);
	//m_value = value;
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::replace(const std::basic_string<TChar>& strold, const std::basic_string<TChar>& strnew)
{
	return replace(strold.c_str(), strnew.c_str());
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::replace(const std::basic_string<TChar>& strold, const TChar* strnew)
{
	return replace(strold.c_str(), strnew);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::replace(const TChar* strold, const std::basic_string<TChar>& strnew)
{
	return replace(strold, strnew.c_str());
}

template<class TChar>
inline str_t<TChar> str_t<TChar>::substr(int first, int last) const
{
	int len = (int)length();
	if(len == 0)
		return str_t();
	if(first < 0)
		first = len + first;
	if(last <= 0)
		last = len + last;
	
	if(first > last || first < 0 || last < 0)
		return str_t();

	str_t new_str;
	new_str.resize(last - first + 1);
	gtl::string::tcsncpy(new_str.m_value, m_value + first, last - first);
	return new_str;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::cpy(const TChar* val, size_t count)
{
	if(val == NULL || count == 0)
		return *this;

	resize(count + 1);
	gtl::string::tcsncpy(m_value, val, count);

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::cat(const TChar* val, size_t count)
{
	if(val == NULL || count == 0)
		return *this;

	resize(size() + count + 1);
	gtl::string::tcsncat(m_value, val, count);

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::mcpy(size_t offset, const char* val, size_t size)
{
	if(m_value == NULL || val == NULL || size == 0 || offset >= m_length || offset + size > m_length)
		return *this;

	memcpy(m_value + offset, val, size);
	return *this;
}

template<class TChar>
inline bool str_t<TChar>::cmp(const TChar* val, size_t count) const
{
	if(count == 0)
		return operator==(val);

	if(count == (size_t)-1)
		count = gtl::string::tcslen(val);
	
	return gtl::string::tcsncmp(m_value, val, count) == 0;
}

template<class TChar>
inline bool str_t<TChar>::cmp(const str_t& val, size_t count) const
{
	if(count == 0)
		return operator==(val);

	if(count == (size_t)-1)
		count = val.length();

	return gtl::string::tcsncmp(m_value, val.m_value, count) == 0;
}

template<class TChar>
inline bool str_t<TChar>::cmp(const std::basic_string<TChar>& val, size_t count) const
{
	if(count == 0)
		return operator==(val);

	if(count == (size_t)-1)
		count = val.length();

	return gtl::string::tcsncmp(m_value, val.c_str(), count) == 0;
}

template<class TChar>
inline bool str_t<TChar>::icmp(const TChar* val, size_t count) const
{
	if(m_value == NULL || val == NULL)
		return m_value == val;

	if(count == 0)
		return gtl::string::tcsicmp(m_value, val) == 0;
	else if(count == (size_t)-1)
		count = gtl::string::tcslen(val);

	return gtl::string::tcsnicmp(m_value, val, count) == 0;
}

template<class TChar>
inline bool str_t<TChar>::icmp(const str_t& val, size_t count) const
{
	if(this == &val)
		return true;
	if(m_value == NULL || val.m_value == NULL)
		return m_value == val.m_value;

	if(count == 0)
		return gtl::string::tcsicmp(m_value, val.m_value) == 0;
	else if(count == (size_t)-1)
		count = val.length();

	return gtl::string::tcsnicmp(m_value, val.m_value, count) == 0;
}

template<class TChar>
inline bool str_t<TChar>::icmp(const std::basic_string<TChar>& val, size_t count) const
{
	if(m_value == NULL || val.empty())
		return m_value == NULL && val.empty();

	if(count == 0)
		return gtl::string::tcsicmp(m_value, val.c_str()) == 0;
	else if(count == (size_t)-1)
		count = val.length();

	return gtl::string::tcsnicmp(m_value, val.c_str(), count) == 0;
}

template<class TChar>
inline size_t str_t<TChar>::find(TChar val, size_t start) const
{
	if(m_value == NULL || start + 1 >= length())
		return -1;

	TChar* p = gtl::string::tcschr(m_value + start, val);
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
inline size_t str_t<TChar>::find(const TChar* val, size_t start) const
{
	if(m_value == NULL || val == NULL || start + 1 >= length())
		return -1;

	TChar* p = gtl::string::tcsstr(m_value + start, val);
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
inline size_t str_t<TChar>::find(const str_t& val, size_t start) const
{
	if(m_value == NULL || val.m_value == NULL || start + 1 >= length())
		return -1;

	if(this == &val)
		return 0;

	TChar* p = gtl::string::tcsstr(m_value + start, val.m_value);
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
inline size_t str_t<TChar>::find(const std::basic_string<TChar>& val, size_t start) const
{
	if(m_value == NULL || val.empty() || start + 1 >= length())
		return -1;

	TChar* p = gtl::string::tcsstr(m_value + start, val.c_str());
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
inline size_t str_t<TChar>::find(const TChar** delimiters, int count, const TChar** delimiter, size_t start) const
{
	if(m_value == NULL || delimiters == NULL || count <= 0 || start + 1 >= length())
		return -1;

	TChar* p = find(m_value + start, delimiters, count, delimiter);
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
template<class TStr, template<class, class> class Container>
size_t str_t<TChar>::find(const Container<TStr, std::allocator<TStr> >& delimiters, const TChar** delimiter, size_t start) const
{
	if(m_value == NULL || delimiters.empty() || start + 1 >= length())
		return -1;

	const TChar* dlmtrs[512] = {0};
	Container<TStr, std::allocator<TStr> >::const_iterator first = delimiters.begin();
	Container<TStr, std::allocator<TStr> >::const_iterator last = delimiters.end();
	for(int i = 0; first != last; ++first, ++i)
	{
		dlmtrs[i] = *first;
	}

	return find(dlmtrs, delimiters.size(), delimiter, start);
}

template<class TChar>
inline size_t str_t<TChar>::rfind(TChar val) const
{
	if(m_value == NULL)
		return -1;

	TChar* p = gtl::string::tcsrchr(m_value, val);
	if(p == NULL)
		return -1;

	return p - m_value;
}

template<class TChar>
inline size_t str_t<TChar>::rfind(const TChar* val) const
{
	if(m_value == NULL || val == NULL || *m_value == 0 || *val == 0)
		return -1;

	TChar* s1 = NULL;
	const TChar* s2 = NULL;
	for(int i = (int)length() - 1; i >= 0; --i)
	{
		s1 = m_value + i;
		s2 = val;
		while(*s1 && *s2 && !(*s1-*s2))
			s1++, s2++;
		if(!*s2)
			return (size_t)i;
	}

	return -1;
}

template<class TChar>
inline size_t str_t<TChar>::rfind(const str_t& val) const
{
	if(m_value == NULL || val.m_value == NULL)
		return -1;

	if(this == &val)
		return 0;

	return rfind(val.m_value);
}

template<class TChar>
inline size_t str_t<TChar>::rfind(const std::basic_string<TChar>& val) const
{
	if(m_value == NULL || val.empty())
		return -1;

	return rfind(val.c_str());
}

#ifdef _WINDOWS_
template<class TChar>
inline str_t<typename str_t<TChar>::trans_type> str_t<TChar>::trans(unsigned int codepage) const
{
	str_t<trans_type> strret;
	return trans(strret, codepage);
}

namespace
{

struct local_trans
{
	static inline void trans(const str_t<wchar>& wstr, str_t<char>& cstr, unsigned int codepage)
	{
		if(wstr.empty())
			return;

		int wlen = (int)wstr.length();
		int len = ::WideCharToMultiByte(codepage, 0, wstr.data(), wlen, NULL, NULL, NULL, NULL);
		cstr.resize(len + 1);
		int result = ::WideCharToMultiByte(codepage, 0, wstr.data(), wlen, &cstr[0], len, NULL, NULL);
		(void)result;
	}

	static inline void trans(const str_t<char>& cstr, str_t<wchar>& wstr, unsigned int codepage)
	{
		if(cstr.empty())
			return;

		int len = (int)cstr.length();
		int wlen = ::MultiByteToWideChar(codepage, 0, cstr.data(), len, NULL, NULL);
		wstr.resize(wlen + 1);
		int result = ::MultiByteToWideChar(codepage, 0, cstr.data(), len, &wstr[0], wlen);
		(void)result;
	}

	static inline void utf8(const str_t<wchar>& wstr, str_t<char>& cstr)
	{
		if(wstr.empty())
			return;

		wstr.trans(cstr, CP_UTF8);
	}

	static inline void utf8(const str_t<char>& src, str_t<char>& cstr)
	{
		if(src.empty())
			return;

		src.trans().trans(cstr, CP_UTF8);
	}
};

}  // end of namespace

template<class TChar>
inline const str_t<typename str_t<TChar>::trans_type>& str_t<TChar>::trans(str_t<trans_type>& val, unsigned int codepage) const
{
	local_trans::trans(*this, val, codepage);
	return val;
}

template<class TChar>
inline str_t<char> str_t<TChar>::utf8() const
{
	str_t<char> strret;
	local_trans::utf8(*this, strret);
	return strret;
}
#endif // _WINDOWS_

template<class TChar>
inline str_t<TChar>& str_t<TChar>::ltrim()
{
	if(empty())
		return *this;

	gtl::string::tcsrev(m_value);
	rtrim();
	gtl::string::tcsrev(m_value);
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::rtrim()
{
	if(empty())
		return *this;

	int index = (int)length() - 1;
	for(; index >= 0; --index)
	{
		TChar c = *(m_value + index);
		if(!(c == ' ' || c == '\t' || c == '\r' || c == '\n'))
			break;
	}

	*(m_value + index + 1) = 0;
	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::trim()
{
	return rtrim().ltrim();
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::ltrim(const TChar* val)
{
	if(empty() || val == NULL || *val == 0)
		return *this;

	reverse();
	rtrim(str_t(val).reverse());
	reverse();

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::rtrim(const TChar* val)
{
	if(empty() || val == NULL || *val == 0)
		return *this;

	int i = (int)length() - 1;
	int j = (int)gtl::string::tcslen(val) - 1;
	while(i >= 0 && j >= 0 && *(m_value + i) == *(val + j))
	{
		--i;
		--j;
	}

	if(j < 0)
		*(m_value + i + 1) = 0;

	return *this;
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::trim(const TChar* val)
{
	return rtrim(val).ltrim(val);
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::ltrim(const std::basic_string<TChar>& val)
{
	return ltrim(val.c_str());
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::rtrim(const std::basic_string<TChar>& val)
{
	return rtrim(val.c_str());
}

template<class TChar>
inline str_t<TChar>& str_t<TChar>::trim(const std::basic_string<TChar>& val)
{
	return trim(val.c_str());
}

template<class TChar>
template<class TStr, template<class, class> class Container>
Container<TStr, std::allocator<TStr> >& str_t<TChar>::split(Container<TStr, std::allocator<TStr> >& sub_strs, const TChar* delimiter) const
{
	if(m_value == NULL)
		return sub_strs;

	using namespace gtl::string;

	TChar* express = m_value;
	TChar* pos(NULL);
	TChar* substr(NULL);
	size_t dlmlen = tcslen(delimiter);
	int len(0);

	while(1)
	{
		pos = tcsstr(express, delimiter);
		if(pos == NULL)
		{
			if(tcslen(express) != 0)
				sub_strs.push_back(TStr(express));
			break;
		}
		else
		{
			len = int(pos - express);
			if(len > 0)
			{
				substr = new TChar[len + 1];
				tcsncpy(substr, express, len);
				substr[len] = 0;
				sub_strs.push_back(TStr(substr));
				delete[] substr;
			}
			express = pos + dlmlen;
		}
	}

	return sub_strs;
}

template<class TChar>
template<class TStr, template<class, class> class Container>
Container<TStr, std::allocator<TStr> >& str_t<TChar>::split(Container<TStr, std::allocator<TStr> >& sub_strs, const TChar* delimiter, int count) const
{
	if(m_value == NULL)
		return sub_strs;

	using namespace gtl::string;

	TChar* express = m_value;
	TChar* pos(NULL);
	TChar* substr(NULL);
	size_t dlmlen = tcslen(delimiter);
	int len(0);
	int cn(0);

	while(1)
	{
		pos = tcsstr(express, delimiter);
		if(pos == NULL)
		{
			if(tcslen(express) != 0)
				sub_strs.push_back(TStr(express));
			break;
		}
		else
		{
			len = int(pos - express);
			if(len > 0)
			{
				substr = new TChar[len + 1];
				tcsncpy(substr, express, len);
				substr[len] = 0;
				sub_strs.push_back(TStr(substr));
				delete[] substr;
			}

			express = pos + dlmlen;

			if(count > 0 && ++cn == count)
			{
				sub_strs.push_back(TStr(express));
				break;
			}
		}
	}

	return sub_strs;
}

template<class TChar>
template<class TStr, template<class, class> class Container>
Container<TStr, std::allocator<TStr> >& str_t<TChar>::split(Container<TStr, std::allocator<TStr> >& sub_strs, const TChar** delimiters, int count) const
{
	if(m_value == NULL)
		return sub_strs;

	using namespace gtl::string;

	TChar* express = m_value;
	TChar* pos = NULL;
	TChar* substr = NULL;
	const TChar* delimiter = NULL;
	int len(0);

	while(1)
	{
		pos = find(express, delimiters, count, &delimiter);
		if(pos == NULL)
		{
			if(tcslen(express) != 0)
				sub_strs.push_back(TStr(express));

			break;
		}
		else
		{
			len = int(pos - express);
			if(len > 0)
			{
				substr = new TChar[len + 1];
				tcsncpy(substr, express, len);
				substr[len] = '\0';
				sub_strs.push_back(TStr(substr));
				delete[] substr;
			}

			express = pos + tcslen(delimiter);
		}
	}

	return sub_strs;
}

template<class TChar>
str_t<TChar> str_t<TChar>::split(const TChar* delimiter, int index) const
{
	if(delimiter == NULL || index < -1)
		return str_t();

	using namespace gtl::string;

	int idx = 0;
	int len = 0;
	TChar* pos = NULL;
	TChar* substr = NULL;
	TChar* express = m_value;
	size_t dlmlen = tcslen(delimiter);

	while(1)
	{
		pos = tcsstr(express, delimiter);
		if(pos == NULL)
		{
			if(idx == index || index == -1)
				return express;

			return str_t();
		}

		len = int(pos - express);
		if(len > 0 && idx == index)
		{
			substr = new TChar[len + 1];
			tcsncpy(substr, express, len);
			substr[len] = 0;
			str_t result = substr;
			delete[] substr;

			return result;
		}

		if(index >= 0 && idx++ >= index)
			break;

		express = pos + dlmlen;
	}

	return str_t();
}

template<class TChar>
TChar* str_t<TChar>::find(const TChar* src, const TChar** delimiters, int count, const TChar** valid_delimiter = NULL) const
{
	if(!src || !*src)
		return NULL;

	if(!count || !delimiters || !*delimiters)
		return (TChar*)src;

	TChar *cp = (TChar*)src;
	TChar *s1, *s2;

	while (*cp)
	{
		for(int i = 0; i < count; ++i)
		{
			s1 = cp;
			s2 = (TChar*)delimiters[i];
			if(!s2 || !*s2)
				continue;

			while ( *s1 && *s2 && !(*s1-*s2) )
				s1++, s2++;

			if (!*s2)
			{
				if(valid_delimiter)
					*valid_delimiter = delimiters[i];
				return(cp);
			}
		}

		cp++;
	}

	return NULL;
}

template<class TChar>
template<class TStr, template<class, class> class Container>
str_t<TChar> str_t<TChar>::join(Container<TStr, std::allocator<TStr> >& strs) const
{
	if(strs.empty())
		return str_t();

	return join(strs, 0, strs.size());
}

template<class TChar>
template<class TStr, template<class, class> class Container>
str_t<TChar> str_t<TChar>::join(Container<TStr, std::allocator<TStr> >& strs, size_t index, size_t count) const
{
	if(strs.empty() || index >= strs.size())
		return str_t();

	str_t new_str;
	size_t cnt = min(index + count, strs.size());

	if(empty())
	{
		for(size_t i = index; i < cnt; ++i)
		{
			new_str += strs[i];
		}
	}
	else
	{
		--cnt;
		for(size_t i = index; i < cnt; ++i)
		{
			new_str += strs[i] + m_value;
		}

		new_str += strs[cnt];
	}

	return new_str;
}

template<class TChar>
str_t<TChar> str_t<TChar>::join(const TChar** strs, size_t index, size_t count) const
{
	if(strs == NULL || index >= count)
		return str_t();

	str_t new_str;
	if(empty())
	{
		for(size_t i = index; i < count; ++i)
		{
			new_str += strs[i];
		}
	}
	else
	{
		--count;
		for(size_t i = index; i < count; ++i)
		{
			new_str += strs[i];
			new_str += m_value;
		}

		new_str += strs[count];
	}

	return new_str;
}

} // end of namespace gtl

#pragma warning(pop)