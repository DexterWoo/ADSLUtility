#ifndef _INNER_TYPE_H_
#define _INNER_TYPE_H_

//////////////////////////////////////////////////////////////////////////
// 文件名:			inner_type.h
// 功能:			对所有内嵌型别进行一层包装,使其被定义的时候初始化为0值
// 作者:			卢三星
// 创建日期:		2006-03-09
// 更新日期:		2008-12-05
//////////////////////////////////////////////////////////////////////////

namespace gtl 
{

#ifdef _UNICODE
	typedef wchar_t			wchar;
	typedef wchar_t			tchar;
#else
	typedef wchar_t			wchar;
	typedef char			tchar;
#endif

namespace type 
{

template<class Inner>
struct inner
{
	typedef Inner type;
	Inner value;

	inner() : value(0)
	{
	}

	inner(Inner v) : value(v)
	{
	}

	operator Inner&()
	{
		return value;
	}

	operator Inner() const
	{
		return value;
	}

	template<class T> inline bool operator >  (T t) const { return value > t;  }
	template<class T> inline bool operator <  (T t) const { return value < t;  }
	template<class T> inline bool operator >= (T t) const { return value >= t; }
	template<class T> inline bool operator <= (T t) const { return value <= t; }
	template<class T> inline bool operator == (T t) const { return value == t; }
	template<class T> inline bool operator != (T t) const { return value != t; }

	template<class T> inline T operator + (T t) const { return value + t; }
	template<class T> inline T operator - (T t) const { return value - t; }
	template<class T> inline T operator * (T t) const { return value * t; }
	template<class T> inline T operator / (T t) const { return value / t; }
	template<class T> inline T operator & (T t) const { return value & t; }
	template<class T> inline T operator | (T t) const { return value | t; }
	template<class T> inline T operator ^ (T t) const { return value ^ t; }

	template<class T> inline inner& operator += (T t) { value += t; return *this; }
	template<class T> inline inner& operator -= (T t) { value -= t; return *this; }
	template<class T> inline inner& operator *= (T t) { value *= t; return *this; }
	template<class T> inline inner& operator /= (T t) { value /= t; return *this; }
	template<class T> inline inner& operator &= (T t) { value &= t; return *this; }
	template<class T> inline inner& operator |= (T t) { value |= t; return *this; }
	template<class T> inline inner& operator ^= (T t) { value ^= t; return *this; }
};

} // end namespace type

} // end namespace gtl

#ifndef USE_INNER_TYPE
typedef signed char							int8;
typedef unsigned char						byte;
typedef unsigned char						uint8;
typedef signed short						int16;
typedef unsigned short						uint16;
typedef unsigned short						ushort;
typedef signed long							int32;
typedef unsigned long						uint32;
typedef __int64								int64;
typedef unsigned __int64					uint64;
typedef unsigned int						uint;
typedef unsigned long						ulong;
typedef unsigned char						uchar;
typedef float								real;
typedef float								real8;
typedef double								real16;
#else
typedef gtl::type::inner<signed char>		int8;
typedef gtl::type::inner<unsigned char>		byte;
typedef gtl::type::inner<unsigned char>		uint8;
typedef gtl::type::inner<signed short>		int16;
typedef gtl::type::inner<unsigned short>	uint16;
typedef gtl::type::inner<unsigned short>	ushort;
typedef gtl::type::inner<signed long>		int32;
typedef gtl::type::inner<unsigned long>		uint32;
typedef gtl::type::inner<__int64>			int64;
typedef gtl::type::inner<unsigned __int64>  uint64;
typedef gtl::type::inner<unsigned int>		uint;
typedef gtl::type::inner<unsigned long>		ulong;
typedef gtl::type::inner<unsigned char>		uchar;
typedef gtl::type::inner<float>				real;
typedef gtl::type::inner<float>				real8;
typedef gtl::type::inner<double>			real16;
#endif

typedef gtl::wchar wchar;
typedef gtl::tchar tchar;

#endif