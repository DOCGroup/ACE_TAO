// -*- C++ -*-   $Id$
// This file collects the type definitions for data types
// used in ACE XML parser.

#ifndef _ACEXML_XML_TYPES_H_
#define _ACEXML_XML_TYPES_H_
#include "ace/OS.h"
#include "ace/SString.h"
#include "common/ACEXML_Export.h"

#if defined (ACE_HAS_WCHAR) && (ACE_SIZEOF_WCHAR == 2)
typedef wchar_t ACEXML_UTF16;
#else
typedef ACE_USHORT16 ACEXML_UTF16;
#endif /* ACE_HAS_WCHAR && ACE_SIZEOF_WCHAR == 2 */

#if defined (ACE_HAS_WCHAR) && (ACE_SIZEOF_WCHAR == 4)
typedef wchar_t ACEXML_UCS4;
#else
typedef ACE_UINT32 ACEXML_UCS4;
#endif /* ACE_HAS_WCHAR && ACE_SIZEOF_WCHAR == 4 */

typedef char ACEXML_UTF8;

typedef ACEXML_UTF8 ACEXML_Char;
// Default XML character encoding.  We can change this to either
// ACEXML_UTF8 or ACEXML_UTF16

#if 0  // defined (ACE_HAS_STANDARD_CPP_LIBRARY)
typedef string ACEXML_String;
#else
typedef ACE_CString ACEXML_String;
// Defulat XML strings will use UTF-8 encoding.  We would like to use
// the string classes in standard C++ Library here.  But they are not
// very portable yet (as far as I know,) and I'll just use ACE_CString
// for now.  Notice that you should never convert strings between
// ACE_CString and ACE_WString using the built-in conversion functions
// as they only perform simple copy without any encoding conversion.
#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

#if (!defined (ACEXML_HAS_INLINE) && defined (__ACE_INLINE__)) ||  (ACEXML_HAS_INLINE == 1)
#  define __ACEXML_INLINE__ inline
#else
# if defined (__ACEXML_INLINE__)
#   undefine __ACEXML_INLINE__
# endif /* __ACEXML_INLINE__ */
#endif /* (!ACEXML_HAS_INLINE) && (__ACE_INLINE__) || (ACEXML_HAS_INLINE == 1) */

#if defined (__ACEXML_INLINE__)
# define ACEXML_INLINE inline
#else
# define ACEXML_INLINE
#endif /* __ACEXML_INLINE */

// As we always assume sizeof<char> on all platforms are 1 byte long,
// so char == UTF8 char, and there is no need to define it again.
#endif /* _ACEXML_XML_TYPE_H_ */
