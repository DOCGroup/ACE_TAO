// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Types.h
 *
 * This file collects the type definitions for data types
 * used in ACE XML parser.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_XML_TYPES_H_
#define _ACEXML_XML_TYPES_H_

#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Basic_Types.h"
#include "ace/SString.h"
#include "ace/Functor_String.h"

# if defined (ACE_HAS_WCHAR) && (ACE_SIZEOF_WCHAR == 2)
using ACEXML_UTF16 = wchar_t;
# else
using ACEXML_UTF16 = ACE_UINT16;
# endif /* ACE_HAS_WCHAR && ACE_SIZEOF_WCHAR == 2 */

# if defined (ACE_HAS_WCHAR) && (ACE_SIZEOF_WCHAR == 4)
using ACEXML_UCS4 = wchar_t;
# else
using ACEXML_UCS4 = ACE_UINT32;
# endif /* ACE_HAS_WCHAR && ACE_SIZEOF_WCHAR == 4 */

using ACEXML_UTF8 = char;

/**
 * @typedef ACEXML_Char
 * ACEXML_Char only maps to ACEXML_UTF16 when ACE_USES_WCHAR is defined.
 * Here we assume sizeof (wchar_t) is always 2 bytes.
 *
 * Default XML strings will use UTF-8 encoding.  We would like to use
 * the string classes in standard C++ Library here.  But they are not
 * very portable yet (as far as I know,) and I'll just use ACE_CString
 * for now, unless UNICODE support is turned on.  Notice that you
 * should never convert strings between ACE_CString and ACE_WString
 * using the built-in conversion functions as they only perform simple
 * copy without any encoding conversion.
 */
using ACEXML_Char = ACE_TCHAR;
# if defined (ACE_USES_WCHAR)
//typedef ACEXML_UTF16 ACEXML_Char;
using ACEXML_String = ACE_WString;
# else
//typedef ACEXML_UTF8 ACEXML_Char;
using ACEXML_String = ACE_CString;
# endif /* ACE_USES_WCHAR */

# if (!defined (ACEXML_HAS_INLINE) && defined (__ACE_INLINE__)) ||  (ACEXML_HAS_INLINE == 1)
#   define __ACEXML_INLINE__ inline
# else
#   if defined (__ACEXML_INLINE__)
#     undef __ACEXML_INLINE__
#   endif /* __ACEXML_INLINE__ */
# endif /* (!ACEXML_HAS_INLINE) && (__ACE_INLINE__) || (ACEXML_HAS_INLINE == 1) */

# if defined (__ACEXML_INLINE__)
#   define ACEXML_INLINE inline
# else
#   define ACEXML_INLINE
# endif /* __ACEXML_INLINE */


#include /**/ "ace/post.h"

#endif /* _ACEXML_XML_TYPE_H_ */
