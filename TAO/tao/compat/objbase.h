/* This may look like C, but it's really -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    objbase.h
//
// = DESCRIPTION
// 
//   This has "enough COM" to compile (on UNIX) a recent version of the
//   IIOP engine code, which more or less uses COM as a source standard.
//
//   (It's unknown whether the C++ compilers on UNIX put vtables as the first
//   member or not; that matters to folk writing "objects in C", relying on the
//   COM ABI.  In any case, the API to a pure C++ programmer is the same.)
//
//   Note that Microsoft changed the header file structure so that some
//   development environments now prefer "objbase.h" not "compobj.h".
//
// = AUTHOR
//     Portions Copyright 1994-1995 by Sun Microsystems Inc.
//     Portions Copyright 1997 by Washington University
//
// ============================================================================

#ifndef _compobj_h
#define _compobj_h

#  if !defined(ACE_OS_H)
#    error You did not include "ace/OS.h" before objbase.h.  This will break on WIN32 platforms.
#  endif

//
// Random data types that come from Microsoft's APIs.
//
typedef u_long ULONG;
typedef u_long TAO_HRESULT;

//
// Details of the procedure calling convention matter.
//
//#define __stdcall                // MS-Windows non-varargs call convention

#if defined(__cplusplus)
#  define EXTERN_C extern "C"
#else
#  define EXTERN_C extern
#endif

//
// IDs come built from UUIDs
//      IID -- interface ID
//      CLSID -- implementation ID
//
typedef char TAO_IID [16];          // XXX actually a struct
typedef const TAO_IID &TAO_REFIID;

// #if !defined(INITGUID)
// #  define TAO_DEFINE_GUID(name,b,c,d,e,f,g,h,i,j,k,l) \
//                       extern "C" const TAO_IID name
// #else
#  define TAO_DEFINE_GUID(name,b,c,d,e,f,g,h,i,j,k,l) \
                      extern "C" const TAO_IID name = { 0 }
// #endif


//
// All objects in the "Component Object Model" (COM) inherit from
// this pure virtual base class.
//
extern "C" const TAO_IID IID_TAO_IUnknown;

class TAO_IUnknown
{
public:
  virtual TAO_HRESULT  QueryInterface (TAO_REFIID riid,
                                   void **ppv) = 0;
  virtual ULONG  AddRef (void) = 0;
  virtual ULONG  Release (void) = 0;
};

// XXX haven't looked closely at COM's fault reporting yet ...

typedef u_long TAO_SCODE;

inline TAO_HRESULT
TAO_ResultFromScode(TAO_SCODE scode)
{
  return scode;
}

#define TAO_NOERROR         ((TAO_SCODE) 0)
#if !defined(NOERROR)
#  define NOERROR TAO_NOERROR
#endif /* ! NOERROR */
#define TAO_E_NOINTERFACE   ((TAO_SCODE) 17)


// VARIANT is OLE's extremely limited version of "Any".  There are
// numerous definitions of this type in different MS environments;
// this one's the Win32 version.  We need it here because part of the
// CORBA-COM mapping is conversions between Any and VARIANT.
//
// XXX not all of these types are worth trying to support portably,
// and even those that are worth supporting portably aren't fully
// supported at this writing.

typedef u_short         TAO_VARTYPE;

#if 0
enum TAO_VARENUM
{                  // only types suitable for VARIANTs
  VT_EMPTY    = 0,            // nothing
  VT_NULL     = 1,            // SQL style ull (XXX)
  VT_I2       = 2,            // signed 2 byte integer
  VT_I4       = 3,            // signed 4 byte integer
  VT_R4       = 4,            // IEEE "single" floating point
  VT_R8       = 5,            // IEEE "double" floating point
  VT_CY       = 6,            // currency
  VT_DATE     = 7,            // date
  VT_BSTR     = 8,            // counted string
  VT_DISPATCH = 9,            // IDispatch COM object
  VT_ERROR    = 10,           // SCODE
  VT_BOOL     = 11,           // true=1, false=0
  VT_VARIANT  = 12,           // variant pointer
  VT_UNKNOWN  = 13,           // IUnknown COM object
  VT_UI1      = 17,           // octet
  VT_ARRAY    = 0x2000,       // SAFEARRAY * of more primitive type
  VT_BYREF    = 0x4000        // pointer to more primitive type
};
#endif /* 0 */

struct TAO_CY
  // = TITLE
  //   Currency is an eight byte fixed point number (could be "long long").
{
#if defined (WORDS_BIGENDIAN)
    long Hi;
    u_long Lo;
#else
    u_long Lo;
    long Hi;
#endif /* WORDS_BIGENDIAN */
};

// DATE format is days since 30-Dec-1889 ... days in the "whole"
// part, time in the fractional part (part of a day).

typedef double TAO_DATE;

struct TAO_VARIANT
{
  TAO_VARTYPE               vt;             // type ID
  u_short               wReserved1, wReserved2, wReserved3;
  union
  {
    //
    // By-Value fields
    //
    long                lVal;                   // VT_I4
    u_char              bVal;                   // VT_UI1
    short               iVal;                   // VT_I2
    float               fltVal;                 // VT_R4
    double              dblVal;                 // VT_R8
    // VARIANT_BOOL     bool;                   // VT_BOOL
    TAO_SCODE           scode;                  // VT_ERROR
    TAO_CY              cyVal;                  // VT_CY
    TAO_DATE            date;                   // VT_DATE
    // BSTR             bstrVal;                // VT_BSTR
    TAO_IUnknown        *punkVal;               // VT_UNKNOWN
    // IDispatch        *pdispVal;              // VT_DISPATCH
    // SAFEARRAY        *parray;                // VT_ARRAY

    //
    // By-Reference fields (for VARIANTARG)
    //
    u_char              *pbVal;                 // VT_BYREF|VT_UI1
    short               *piVal;                 // VT_BYREF|VT_I2
    long                *plVal;                 // VT_BYREF|VT_I4
    float               *pfltVal;               // VT_BYREF|VT_R4
    double              *pdblVal;               // VT_BYREF|VT_R8
    // VARIANT_BOOL     *pbool;                 // VT_BYREF|VT_BOOL
    TAO_SCODE           *pscode;                // VT_BYREF|VT_ERROR
    TAO_CY              *pcyVal;                // VT_BYREF|VT_CY
    TAO_DATE            *pdate;                 // VT_BYREF|VT_DATE
    // BSTR             *pbstrVal;              // VT_BYREF|VT_BSTR
    TAO_IUnknown            **ppunkVal;             // VT_BYREF|VT_UNKNOWN
    // IDispatch        **ppdisVal;             // VT_BYREF|VT_DISPATCH
    // SAFEARRAY        *pparray;               // VT_BYREF|VT_ARRAY

    TAO_VARIANT         *pvarVal;               // VT_BYREF|VT_VARIANT
    void                *byref;                 // generic VT_BYREF
  };
};

#endif  // _compobj_h
