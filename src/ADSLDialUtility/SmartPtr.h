#pragma once
#include <Windows.h>

class CSpCount
{
public:
    CSpCount    (int c = 0) : refCnt(c) {}
    ~CSpCount   ()                      {}

    LONG AddRef (){ return InterlockedExchangeAdd(&refCnt, 1); }
    LONG Release(){ return InterlockedExchangeAdd(&refCnt, -1);}

private:
    LONG refCnt;
};

template <class T>
class CSmartPtr
{
public:
    explicit CSmartPtr (T* ptr) : ptr(ptr), ref(new CSpCount(1)) {}
    explicit CSmartPtr () : ptr(NULL), ref(NULL)  {}

    ~CSmartPtr ()
    {
        if (ptr != NULL && ref->Release() == 1)
        {
            delete ptr;
            delete ref;
            ptr = NULL;
            ref = NULL;
        }       
    }

    CSmartPtr (const CSmartPtr<T>& other)
    {
        ptr = other.ptr;
        ref = other.ref;

        if (ref != NULL) ref->AddRef();
    }

	void operator= (T* tmpPtr)
    {          
        if (ptr && ref->Release() == 1)
        {
            delete ptr;
            delete ref;
            ptr = NULL;
            ref = NULL;
        }
        ptr = tmpPtr;

        if (ptr != NULL) ref = new CSpCount(1);
        else             ref = NULL;
    }
	void operator= (const CSmartPtr<T>& t)
    {          
        if (ptr != NULL && ref->Release() == 1)
        {
            delete ptr;
            delete ref;
            ptr = NULL;
            ref = NULL;
        }

        
        ptr = t.ptr;
        ref = t.ref;

        if (ref != NULL) ref->AddRef();
    }

   
    T* operator ->() { return ptr; }
    T& operator * () { return *ptr;}
       operator T*() { return ptr; }    

private:
    T* ptr;
    CSpCount* ref;
};
// 重载==操作符.
template<class T, class U> inline bool operator==(CSmartPtr<T> & first, CSmartPtr<U> & second)
{
	return (T*)first == (U*)second;
}

// 重载!=操作符.
template<class T, class U> inline bool operator!=(CSmartPtr<T> & first, CSmartPtr<U> & second)
{
	return (T*)first != (U*)second;
}

template <class T>
class CSmartArrayPtr
{
public:
	explicit CSmartArrayPtr(T* ptr) : ptr(ptr), ref(new CSpCount(1)) {}
	explicit CSmartArrayPtr() : ptr(NULL), ref(NULL)  {}

	~CSmartArrayPtr()
	{
		if (ptr != NULL && ref->Release() == 1)
		{
			delete[] ptr;
			delete ref;
			ptr = NULL;
			ref = NULL;
		}
	}

	CSmartArrayPtr(const CSmartPtr<T>& other)
	{
		ptr = other.ptr;
		ref = other.ref;

		if (ref != NULL) ref->AddRef();
	}

	void operator= (T* tmpPtr)
	{
		if (ptr && ref->Release() == 1)
		{
			delete[] ptr;
			delete ref;
			ptr = NULL;
			ref = NULL;
		}
		ptr = tmpPtr;

		if (ptr != NULL) ref = new CSpCount(1);
		else             ref = NULL;
	}
	void operator= (const CSmartArrayPtr<T>& t)
	{
		if (ptr != NULL && ref->Release() == 1)
		{
			delete[] ptr;
			delete ref;
			ptr = NULL;
			ref = NULL;
		}


		ptr = t.ptr;
		ref = t.ref;

		if (ref != NULL) ref->AddRef();
	}


	T* operator ->() { return ptr; }
	T& operator * () { return *ptr; }
	operator T*() { return ptr; }

private:
	T* ptr;
	CSpCount* ref;
};

// 重载==操作符.
template<class T, class U> inline bool operator==(CSmartArrayPtr<T> & first, CSmartArrayPtr<U> & second)
{
    return (T*)first == (U*)second;
}

// 重载!=操作符.
template<class T, class U> inline bool operator!=(CSmartArrayPtr<T> & first, CSmartArrayPtr<U> & second)
{
    return (T*)first != (U*)second;
}