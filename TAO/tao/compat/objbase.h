// @(#)objbase.h	1.4 95/09/25
// Copyright 1995 by Sun Microsystems, Inc.
//
// COM		Basic Component Object Model definitions
//
// This has "enough COM" to compile (on UNIX) a recent version of the
// IIOP engine code, which more or less uses COM as a source standard.
//
// (It's unknown whether the C++ compilers on UNIX put vtables as the first
// member or not; that matters to folk writing "objects in C", relying on the
// COM ABI.  In any case, the API to a pure C++ programmer is the same.)
//
// Note that Microsoft changed the header file structure so that some
// development environments now prefer "objbase.h" not "compobj.h".
//

#ifndef	_compobj_h
#define	_compobj_h

//
// Random data types that come from Microsoft's APIs.
//
typedef unsigned long ULONG;
typedef unsigned long HRESULT;

//
// Details of the procedure calling convention matter.
//
#define __stdcall		// MS-Windows non-varargs call convention

//
// IDs come built from UUIDs
//	IID -- interface ID
//	CLSID -- implementation ID
//
typedef char IID [16];		// XXX actually a struct
typedef const IID &REFIID;
#define DEFINE_GUID(name,b,c,d,e,f,g,h,i,j,k,l) extern "C" const IID name;


//
// All objects in the "Component Object Model" (COM) inherit from
// this pure virtual base class.
//
DEFINE_GUID (IID_IUnknown, b,c,d,e,f,g,h,i,j,k,l)

class IUnknown {
  public:
    virtual HRESULT __stdcall	QueryInterface (
				    REFIID	riid,
				    void	**ppv
				) = 0;
    virtual ULONG __stdcall	AddRef () = 0;
    virtual ULONG __stdcall	Release () = 0;
};



// XXX haven't looked closely at COM's fault reporting yet ...

typedef unsigned long SCODE;

inline HRESULT ResultFromScode(SCODE scode) { return scode; }

#define	NOERROR		((SCODE) 0)
#define E_NOINTERFACE	((SCODE) 17)


//
// VARIANT is OLE's extremely limited version of "Any".  There are
// numerous definitions of this type in different MS environments;
// this one's the Win32 version.  We need it here because part of
// the CORBA-COM mapping is conversions between Any and VARIANT. 
//
// XXX not all of these types are worth trying to support portably,
// and even those that are worth supporting portably aren't fully
// supported at this writing.
//
typedef unsigned short		VARTYPE;
enum VARENUM {			// only types suitable for VARIANTs
    VT_EMPTY	= 0,		// nothing
    VT_NULL	= 1,		// SQL style ull (XXX)
    VT_I2	= 2,		// signed 2 byte integer
    VT_I4	= 3,		// signed 4 byte integer
    VT_R4	= 4,		// IEEE "single" floating point
    VT_R8	= 5,		// IEEE "double" floating point
    VT_CY	= 6,		// currency
    VT_DATE	= 7,		// date
    VT_BSTR	= 8,		// counted string
    VT_DISPATCH	= 9,		// IDispatch COM object
    VT_ERROR	= 10,		// SCODE
    VT_BOOL	= 11,		// true=1, false=0
    VT_VARIANT	= 12,		// variant pointer
    VT_UNKNOWN	= 13,		// IUnknown COM object
    VT_UI1	= 17,		// octet
    VT_ARRAY	= 0x2000,	// SAFEARRAY * of more primitive type
    VT_BYREF	= 0x4000	// pointer to more primitive type
};

//
// Currency is an eight byte fixed point number (could be "long long")
//
struct CY {
#ifdef	WORDS_BIGENDIAN
    long		Hi;
    unsigned long	Lo;
#else
    unsigned long	Lo;
    long		Hi;
#endif
};

//
// DATE format is days since 30-Dec-1889 ... days in the "whole"
// part, time in the fractional part (part of a day).
//
typedef double DATE;

struct VARIANT {
    VARTYPE		vt;		// type ID
    unsigned short	wReserved1, wReserved2, wReserved3;
    union {
	//
	// By-Value fields
	//
	long		lVal;			// VT_I4
	unsigned char	bVal;			// VT_UI1
	short		iVal;			// VT_I2
	float		fltVal;			// VT_R4
	double		dblVal;			// VT_R8
	// VARIANT_BOOL	bool;			// VT_BOOL
	SCODE		scode;			// VT_ERROR
	CY		cyVal;			// VT_CY
	DATE		date;			// VT_DATE
	// BSTR		bstrVal;		// VT_BSTR
	IUnknown	*punkVal;		// VT_UNKNOWN
	// IDispatch	*pdispVal;		// VT_DISPATCH
	// SAFEARRAY	*parray;		// VT_ARRAY

	//
	// By-Reference fields (for VARIANTARG)
	//
	unsigned char	*pbVal;			// VT_BYREF|VT_UI1
	short		*piVal;			// VT_BYREF|VT_I2
	long		*plVal;			// VT_BYREF|VT_I4
	float		*pfltVal;		// VT_BYREF|VT_R4
	double		*pdblVal;		// VT_BYREF|VT_R8
	// VARIANT_BOOL	*pbool;			// VT_BYREF|VT_BOOL
	SCODE		*pscode;		// VT_BYREF|VT_ERROR
	CY		*pcyVal;		// VT_BYREF|VT_CY
	DATE		*pdate;			// VT_BYREF|VT_DATE
	// BSTR		*pbstrVal;		// VT_BYREF|VT_BSTR
	IUnknown	**ppunkVal;		// VT_BYREF|VT_UNKNOWN
	// IDispatch	**ppdisVal;		// VT_BYREF|VT_DISPATCH
	// SAFEARRAY	*pparray;		// VT_BYREF|VT_ARRAY

	VARIANT		*pvarVal;		// VT_BYREF|VT_VARIANT
	void		*byref;			// generic VT_BYREF
    };
};

#endif	// _compobj_h
