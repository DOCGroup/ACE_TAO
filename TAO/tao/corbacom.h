// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    corbacom.h
//
// = DESCRIPTION
//    CORBA C/C++/COM mapping for Win32
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_CORBACOM_H)
#define TAO_CORBACOM_H

#include <objbase.h>		// Win32 name for "compobj.h"

#if	SIZEOF_BOOL != 0
typedef bool			CORBA_Boolean;
#define CORBA_B_FALSE false
#define CORBA_B_TRUE true

#else	/* "bool" not builtin to this compiler */
typedef int			CORBA_Boolean;
enum { CORBA_B_FALSE = 0, CORBA_B_TRUE = 1 };
#endif /* "bool" not builtin */

typedef u_char CORBA_Octet;

typedef short CORBA_Short;
typedef u_short CORBA_UShort;

// CORBA "Long" (and its unsigned cousin) are 32 bits, just like on
// almost all C/C++ compilers.

#if	SIZEOF_LONG == 4
typedef long CORBA_Long;
typedef u_long CORBA_ULong;
#else
// just assume "int" is 4 bytes long ...
typedef int CORBA_Long;
typedef u_int CORBA_ULong;
#endif	/* SIZEOF_LONG != 4 */

// 94-9-32 Appendix A, also the OMG C++ mapping, stipulate that 64 bit
// integers are "LongLong".
//
// NOTE:  those are IDL extensions, not yet standard.

#if	SIZEOF_LONG_LONG == 8
typedef long long CORBA_LongLong;
typedef unsigned long long CORBA_ULongLong;
#elif	SIZEOF_LONG == 8
typedef long CORBA_LongLong;
typedef u_long CORBA_ULongLong;
#elif	defined (_MSC_VER) && _MSC_VER >= 900
typedef __int64 CORBA_LongLong;
typedef unsigned __int64 CORBA_ULongLong;
#else

// If "long long" isn't native, programs can't use these data types in
// normal arithmetic expressions.  If any particular application can
// cope with the loss of range it can define conversion operators
// itself.

#   define	NONNATIVE_LONGLONG
#if	defined (WORDS_BIGENDIAN)
struct CORBA_LongLong { CORBA_Long h, l; };
struct CORBA_ULongLong { CORBA_Long h, l; };
#else
struct CORBA_LongLong { CORBA_Long l, h; };
struct CORBA_ULongLong { CORBA_ULong l, h; };
#endif /* !WORDS_BIGENDIAN */
#endif /* no native 64 bit integer type */

typedef float CORBA_Float;
typedef double CORBA_Double;

// 94-9-32 Appendix A defines a 128 bit floating point "long double"
// data type, with greatly extended precision and four more bits of
// exponent (compared to "double").
//
// NOTE:  that is an IDL extension, not yet standard.

#if	SIZEOF_LONG_DOUBLE == 16
typedef long double CORBA_LongDouble;
#else
#define	NONNATIVE_LONGDOUBLE
struct CORBA_LongDouble 
{
  char ld[16];
};
#endif /* SIZEOF_LONG_DOUBLE != 16 */

typedef char CORBA_Char;
typedef CORBA_Char *CORBA_String;

ACE_Svc_Export CORBA_String CORBA_string_alloc (CORBA_ULong len);
CORBA_String CORBA_string_copy (const CORBA_Char *const);
CORBA_String CORBA_string_dup (const CORBA_Char *const);
ACE_Svc_Export void CORBA_string_free (CORBA_Char *const);


class CORBA_String_var
  // = TITLE
  // @@ Please add a comment here.
{
public:
  // @@ Please add comments here.
  CORBA_String_var (void);
  CORBA_String_var (char *p);
  CORBA_String_var (const char *p);
  CORBA_String_var (const CORBA_String_var &s);
  ~CORBA_String_var (void);

  CORBA_String_var &operator= (char *p);
  CORBA_String_var &operator= (const char *p);
  CORBA_String_var &operator= (const CORBA_String_var &s);
  operator char* () { return ptr_; }
  operator const char* () const {return ptr_; };
  char &operator[] (CORBA_ULong index);
  char operator[] (CORBA_ULong index) const;

private:
  char *ptr_;
};

// 94-9-32 Appendix A defines 16-bit UNICODE characters as
// "WChar", and null-terminated strings of them as "WString".
//
// NOTE:  those are IDL extensions, not yet standard.

#if	SIZEOF_WCHAR_T < 2
typedef short CORBA_WChar;
#else
typedef wchar_t CORBA_WChar;
#endif /* SIZEOF _WCHAR_T < 2 */
typedef CORBA_WChar *CORBA_WString;

CORBA_WString CORBA_wstring_alloc (CORBA_ULong len);
CORBA_WString CORBA_wstring_copy (const CORBA_WChar *const);
void CORBA_wstring_free (CORBA_WChar *const);

#endif /* TAO_CORBA_COM_H */
