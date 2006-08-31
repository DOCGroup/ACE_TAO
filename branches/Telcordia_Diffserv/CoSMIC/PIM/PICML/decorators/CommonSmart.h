
#ifndef MGA_COMMONSMART_H
#define MGA_COMMONSMART_H

#ifndef MGA_COMMONERROR_H
#include "CommonError.h"
#endif

#ifndef __ATLBASE_H__
#include <AtlBase.h>
#endif

// --------------------------- CComObjPtr

// in containers we need the default "operator &",
// so we don't use the built in ATL CComPtr class

template<class T>
class CComObjPtr
{
public:
	CComObjPtr() { p = NULL; }
	CComObjPtr(T *q) { if ((p = q) != NULL) q->AddRef(); }
	CComObjPtr(const CComObjPtr<T> &q) { if((p = q.p) != NULL) p->AddRef(); }
#ifdef _DEBUG
 	~CComObjPtr() { if(p) p->Release(); p = NULL; }
#else
	~CComObjPtr() { if(p) p->Release(); }
#endif
	void Release() { if(p) { p->Release(); p = NULL; } }

	operator T*() const { return p; }
	T& operator*() const { ASSERT( p != NULL ); return *p; }
	T* operator->() const { ASSERT( p != NULL ); return p; }
	T* operator=(T *q) { if(q) q->AddRef(); if(p) p->Release(); p = q; return q; }
	T* operator=(const CComObjPtr<T> &q) { return operator=(q.p); }
	bool operator==(T *q) const { return p == q; }
	bool operator!=(T *q) const { return p != q; }

	T* Detach() { T* r = p; p = NULL; return r; }
	void Attach(T* q) { if(p) p->Release(); p = q; }

	template<class Q>
	HRESULT QueryInterface(CComObjPtr<Q> &q) const 
	{
		ASSERT( p != NULL && q == NULL );
		return p->QueryInterface(__uuidof(Q), (void**)&q.p);
	}

	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ASSERT( p == NULL );
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}

	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}

public:
	T *p;
};

// use these inline functions to pass CComObjPtr arguments to COM functions
// like in role->get_Kind(PutOut(kind))

template<class T>
T **PutOut(CComObjPtr<T> &p) { ASSERT( p == NULL ); return &p.p; }

template<class T>
T **PutInOut(CComObjPtr<T> &p) { return &p.p; }

// use these inline functions to transfer ownership to another COM pointer

template<class T, class Q>
void MoveTo(CComObjPtr<T> &p, Q **q)
{ ASSERT( q != NULL ); if(*q) (*q)->Release(); *q = p.p; p.p = NULL; }

template<class T, class Q>
void MoveTo(CComObjPtr<T> &p, CComObjPtr<Q> &q) { q.Attach(p.p); p.p = NULL; }

// use these inline functions to copy COM pointers

template<class T, class Q>
void CopyTo(T *p, Q **q)
{ ASSERT( q != NULL ); if(p) p->AddRef(); if(*q) (*q)->Release(); *q = p; }

template<class T, class Q>
void CopyTo(const CComObjPtr<T> &p, Q **q) 
{ ASSERT( q != NULL ); if(p) p->AddRef(); if(*q) (*q)->Release(); *q = p; }

// global QueryInterface

template<class T>
inline HRESULT QueryInterface(IUnknown *p, T **q)
{ ASSERT( p != NULL ); return p->QueryInterface(__uuidof(T), (void**)q); }

template<class T>
inline HRESULT QueryInterface(IUnknown *p, CComObjPtr<T> &q)
{ ASSERT( p != NULL ); return p->QueryInterface(__uuidof(T), (void**)PutOut(q)); }

// note: T should not be derived from CComObject
template<class T>
inline void CreateComObject(CComObjPtr<T> &p)
{
	CComObject<T> *q = NULL;
	COMTHROW( CComObject<T>::CreateInstance(&q) );
	ASSERT( q != NULL );

	p = q;
}

// note: T should not be derived from CComPartObject
template<class T>
inline void CreateComPartObject(LPUNKNOWN pUnkOuter, CComObjPtr<T> &p)
{
	CComPartObject<T> *q = NULL;
	COMTHROW( CComPartObject<T>::CreateInstance(pUnkOuter, &q) );
	ASSERT( q != NULL );

	p = q;
}

// --------------------------- CComPtr

// note: T should not be derived from CComObject
template<class T>
inline void CreateComObject(CComPtr<T> &p)
{
	CComObject<T> *q = NULL;
	COMTHROW( CComObject<T>::CreateInstance(&q) );
	ASSERT( q != NULL );

	q->AddRef();
	p.Attach(q);
}

// --------------------------- CComBstrObj

class CComBstrObj
{
public:
	CComBstrObj() : p(NULL) { }
	CComBstrObj(LPCOLESTR q) { p = SysAllocString(q); }
	CComBstrObj(const CComBstrObj &q) : p(NULL) { if(q.p) { p = SysAllocStringLen(q, q.Length()); } }
	~CComBstrObj() { if(p) SysFreeString(p); }

	const CComBstrObj &operator=(const CComBstrObj &q) { if(p) SysFreeString(p);
		p = SysAllocStringLen(q, q.Length()); return *this; }
	operator BSTR() const { return p; }
	unsigned int Length() const { return p ? SysStringLen(p) : 0; }
	void Empty() { if( p ) { SysFreeString(p); p = NULL; } }

	int Compare(BSTR q) const;
	bool operator==(BSTR q) const { return Compare(q) == 0; }
	bool operator!=(BSTR q) const { return Compare(q) != 0; }
	bool operator<(BSTR q) const { return Compare(q) < 0; }
	bool operator>(BSTR q) const { return Compare(q) > 0; }
	bool operator<=(BSTR q) const { return Compare(q) <= 0; }
	bool operator>=(BSTR q) const { return Compare(q) >= 0; }

	BSTR Detach() { BSTR q = p; p = NULL; return q; }
	void Attach(BSTR q) { if(p) SysFreeString(p); p = q; }

public:
	BSTR p;
};

inline BSTR *PutOut(CComBstrObj &s) { ASSERT( s.p == NULL ); return &s.p; }
inline BSTR *PutInOut(CComBstrObj &s) { return &s.p; }

// we use "BSTR" for [in], and "BSTR *" for [in, out]
// we use "const CComBstrObj &" for [in] and "CComBstrObj &" for [in, out]

inline void CopyTo(BSTR p, BSTR *q)
{
	ASSERT( q != NULL );

	if(*q) SysFreeString(*q);
	*q = SysAllocStringLen(p, SysStringLen(p));
}

inline void CopyTo(const CComBstrObj &b, BSTR *q) { CopyTo(b.p, q); }
inline void CopyTo(BSTR p, CComBstrObj &b) { CopyTo(p, &b.p); }

inline void MoveTo(BSTR *p, BSTR *q)
{
	ASSERT( p != NULL );
	ASSERT( q != NULL );

	if(*q) SysFreeString(*q);
	(*q) = (*p); (*p) = NULL;
}

inline void MoveTo(CComBstrObj &b, BSTR *q) { MoveTo(&b.p, q); }
inline void MoveTo(BSTR *p, CComBstrObj &b) { MoveTo(p, &b.p); }

// these THROW exceptions

void CopyTo(const char *p, int len, BSTR *b);

// if olelen is -1, then OLECHAR is NULL terminated and the charlen includes this NULL
int GetCharLength(const OLECHAR *p, int olelen);

void CopyTo(const OLECHAR *p, int olelen, char *s, int charlen);

void CopyTo(const OLECHAR *p, GUID &guid);
void CopyTo(const GUID &guid, BSTR *p);

// ------- inlines

inline void CopyTo(const char *p, int len, CComBstrObj &b) { CopyTo(p, len, &b.p); }
inline int GetCharLength(BSTR p) { return GetCharLength(p, SysStringLen(p)); }
inline void CopyTo(BSTR p, char *s, int charlen) { CopyTo(p, SysStringLen(p), s, charlen); }
inline void CopyTo(const GUID &guid, CComBstrObj &b) { CopyTo(guid, &b.p); }

// --------------------------- CComBSTR

inline void CopyTo(const CComBSTR &b, BSTR *q) { CopyTo(b.m_str, q); }
inline void CopyTo(BSTR p, CComBSTR &b) { CopyTo(p, &b.m_str); }

inline void MoveTo(CComBSTR &b, BSTR *q) { MoveTo(&b.m_str, q); }
inline void MoveTo(BSTR *p, CComBSTR &b) { MoveTo(p, &b.m_str); }

// --------------------------- CComSafeArray

class CComSafeArray
{
public:
	CComSafeArray() { p = NULL; }
	~CComSafeArray() { if( p ) SafeArrayDestroy(p); }
	operator SAFEARRAY*() { return p; }

public:
	SAFEARRAY *p;
};

inline SAFEARRAY **PutOut(CComSafeArray &a) { ASSERT( a.p == NULL ); return &a.p; }
inline SAFEARRAY **PutInOut(CComSafeArray &a) { return &a.p; }

// we use "SAFEARRAY *" for [in], "SAFEARRAY **" for [out], and "SAFEARRAY *" for [in, out]
// we use "const CComSafeArray &" for [in] and "CComSafeArray &" for [in, out]

// these THROW exceptions

void CopyTo(const CComBstrObj *start, const CComBstrObj *end, SAFEARRAY **p);
void CopyTo(const unsigned char *start, const unsigned char *end, SAFEARRAY **p);
void CopyTo(const long *start, const long *end, SAFEARRAY **p);
void CopyTo(const GUID *start, const GUID *end, SAFEARRAY **p);

void MoveTo(CComBstrObj *start, CComBstrObj *end, SAFEARRAY **p);

long GetArrayLength(SAFEARRAY *p);

// not safe for multiple access
void GetArrayStart(SAFEARRAY *p, CComBstrObj *&start);
void GetArrayStart(SAFEARRAY *p, unsigned char *&start);
void GetArrayStart(SAFEARRAY *p, long *&start);
void GetArrayStart(SAFEARRAY *p, GUID *&start);

void CopyTo(SAFEARRAY *p, CComBstrObj *start, CComBstrObj *end);
void CopyTo(SAFEARRAY *p, unsigned char *start, unsigned char *end);
void CopyTo(SAFEARRAY *p, long *start, long *end);
void CopyTo(SAFEARRAY *p, GUID *start, GUID *end);

void MoveTo(SAFEARRAY *p, CComBstrObj *start, CComBstrObj *end);

// --------------------------- CComVariant

inline VARIANT *PutOut(CComVariant &v) { ASSERT( v.vt == VT_EMPTY ); return &v; }
inline VARIANT *PutInOut(CComVariant &v) { return &v; }

// we use "const VARIANT &" for [in] and "VARIANT *" for [in, out]
// we use "const CComVariant &" for [in] and "CComVariant &" for [in, out]

// these THROW exceptions

// ------- scalar

void CopyTo(unsigned char a, VARIANT *v);
void CopyTo(short a, VARIANT *v);
void CopyTo(long a, VARIANT *v);
void CopyTo(double a, VARIANT *v);
void CopyTo(BSTR a, VARIANT *v);
void CopyTo(IDispatch *p, VARIANT *v);

template<class T>
void CopyTo(const CComObjPtr<T> &p, VARIANT *v) { CopyTo(p.p, v); }

void CopyTo(const VARIANT &v, unsigned char &a);
void CopyTo(const VARIANT &v, short &a);
void CopyTo(const VARIANT &v, long &a);
void CopyTo(const VARIANT &v, double &a);
void CopyTo(const VARIANT &v, BSTR *a);

void MoveTo(BSTR b, VARIANT *v);
void MoveTo(const VARIANT &v, BSTR *b);

void MoveTo(IDispatch **p, VARIANT *v);

template<class T>
void MoveTo(const CComObjPtr<T> &p, VARIANT *v) { MoveTo(p.p, v); }

inline void MoveTo(VARIANT *p, VARIANT *q)
{
	ASSERT( p != NULL );
	ASSERT( q != NULL );

	(*q) = (*p);
	p->vt = VT_EMPTY;
}

// ------- string conversion

void CopyTo(const char *p, int len, VARIANT *v);

// we cannot use "const VARIANT &", we might have to change
// the type of the variant from VT_DISPATCH to VT_BSTR
int GetCharLength(VARIANT &v);

void CopyTo(const VARIANT &v, char *s, int charlen);

// ------- array conversion

void CopyTo(const CComBstrObj *start, const CComBstrObj *end, VARIANT *v);
void CopyTo(const unsigned char *start, const unsigned char *end, VARIANT *v);
void CopyTo(const long *start, const long *end, VARIANT *v);

void MoveTo(CComBstrObj *start, CComBstrObj *end, VARIANT *v);

long GetArrayLength(const VARIANT &v);

// not safe for multiple access
void GetArrayStart(const VARIANT &v, CComBstrObj *&start);
void GetArrayStart(const VARIANT &v, unsigned char *&start);
void GetArrayStart(const VARIANT &v, long *&start);

void CopyTo(const VARIANT &v, CComBstrObj *start, CComBstrObj *end);
void CopyTo(const VARIANT &v, unsigned char *start, unsigned char *end);
void CopyTo(const VARIANT &v, long *start, long *end);

void MoveTo(VARIANT *v, CComBstrObj *start, CComBstrObj *end);

void CopyTo(const VARIANT &v, GUID &guid);

// ------- inlines

inline void CopyTo(const unsigned char *start, int len, VARIANT *p) { CopyTo(start, start + len, p); }
inline void CopyTo(const long *start, int len, VARIANT *p) { CopyTo(start, start + len, p); }

inline void CopyTo(const GUID &guid, VARIANT *v) { CopyTo((unsigned char*)&guid, sizeof(GUID), v); }
inline void CopyTo(const OBJECTID &objid, VARIANT *v) { CopyTo((unsigned char*)&objid, sizeof(OBJECTID), v); }

inline void CopyTo(const VARIANT &v, CComBstrObj &b) { CopyTo(v, &b.p); }

template<class T>
void CopyTo(T t, CComVariant &v) { CopyTo(t, &v); }

template<class T, class S>
void CopyTo(T t, S s, CComVariant &v) { CopyTo(t, s, &v); }

// --------------------------- COM Helper functions

// these THROW exceptions

bool IsEqualObject(IUnknown *p, IUnknown *q);

// --------------------------- PutInVariant

class PutInVariant
{
public:
	template<class T>
	PutInVariant(T t) { CopyTo(t, v); }

	template<> PutInVariant(int a) : v(a) { }
	template<> PutInVariant(short a) : v(a) { }
	template<> PutInVariant(unsigned char a) : v(a) { }
	template<> PutInVariant(IDispatch *p) : v(p) { }
	template<> PutInVariant(BSTR p) : v(p) { }

	operator VARIANT () { return v; }
	operator CComVariant &() { return v; }

	CComVariant v;
};

// --------------------------- PutInBstr

class PutInBstr
{
public:
	template<class T>
	PutInBstr(T t) { CopyTo(t, b); }

	template<> PutInBstr(BSTR a) : b(a) { }

	operator BSTR () { return b; }

	CComBstrObj b;
};

// --------------------------- CComPartObject

template <class BASE>
class CComPartObject : public BASE
{
public:
	typedef BASE _BaseClass;

	CComPartObject(void* pv)
	{
		ATLASSERT( pv != NULL );
		m_pOuterUnknown = (IUnknown*)pv;
	}

	~CComPartObject()
	{
		FinalRelease();
#ifdef _ATL_DEBUG_INTERFACES
		_Module.DeleteNonAddRefThunk(_GetRawUnknown());
#endif
	}

	STDMETHOD_(ULONG, AddRef)() { return OuterAddRef(); }
	STDMETHOD_(ULONG, Release)() { return OuterRelease(); }

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{ return _InternalQueryInterface(iid, ppvObject); }

	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void**)pp);
	}

	IUnknown* GetControllingUnknown()
	{
#ifdef _ATL_DEBUG_INTERFACES
		IUnknown* p;
		_Module.AddNonAddRefThunk(m_pOuterUnknown, _T("CComContainedObject"), &p);
		return p;
#else
		return m_pOuterUnknown;
#endif
	}

	static HRESULT WINAPI CreateInstance(LPUNKNOWN pUnkOuter, CComPartObject<BASE>** pp)
	{
		ATLASSERT(pp != NULL);
		HRESULT hRes = E_OUTOFMEMORY;
		CComPartObject<BASE>* p = NULL;
		ATLTRY(p = new CComPartObject<BASE>(pUnkOuter))
		if (p != NULL)
		{
			p->SetVoid(NULL);
			hRes = p->FinalConstruct();
			if (hRes != S_OK)
			{
				delete p;
				p = NULL;
			}
		}
		*pp = p;
		return hRes;
	}
};

#endif//MGA_COMMONSMART_H
