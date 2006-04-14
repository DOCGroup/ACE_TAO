// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_ctype.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_CTYPE_H
# define ACE_OS_NS_CTYPE_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

namespace ACE_OS
{

  // these are non-standard names...

  /** @name Functions from <cctype>
   *
   *  Included are the functions defined in <cctype> and their <cwctype>
   *  equivalents.
   *
   *  Since they are often implemented as macros, we don't use the same name
   *  here.  Instead, we change by prepending "ace_".
   */
  //@{

  /// Returns true if the character is an alphanumeric character.
  template <typename CHAR_TYPE> inline
  int ace_isalnum (CHAR_TYPE c);

  /// Returns true if the character is an alphabetic character.
  template <typename CHAR_TYPE> inline
  int ace_isalpha (CHAR_TYPE c);

  /// Returns true if the character is a control character.
  template <typename CHAR_TYPE> inline
  int ace_iscntrl (CHAR_TYPE c);

  /// Returns true if the character is a decimal-digit character.
  template <typename CHAR_TYPE> inline
  int ace_isdigit (CHAR_TYPE c);

  /// Returns true if the character is a printable character other than a space.
  template <typename CHAR_TYPE> inline
  int ace_isgraph (CHAR_TYPE c);

  /// Returns true if the character is a lowercase character.
  template <typename CHAR_TYPE> inline
  int ace_islower (CHAR_TYPE c);

  /// Returns true if the character is a printable character.
  template <typename CHAR_TYPE> inline
  int ace_isprint (CHAR_TYPE c);

  /// Returns true if the character is a punctuation character.
  template <typename CHAR_TYPE> inline
  int ace_ispunct (CHAR_TYPE c);

  /// Returns true if the character is a space character.
  template <typename CHAR_TYPE> inline
  int ace_isspace (CHAR_TYPE c);

  /// Returns true if the character is an uppercase character.
  template <typename CHAR_TYPE> inline
  int ace_isupper (CHAR_TYPE c);

  /// Returns true if the character is a hexadecimal-digit character.
  template <typename CHAR_TYPE> inline
  int ace_isxdigit (CHAR_TYPE c);

  /// Converts a character to lower case (char version).
  ACE_NAMESPACE_INLINE_FUNCTION
  int ace_tolower (int c);

  /// Converts a character to lower case (wchar_t version).
  ACE_NAMESPACE_INLINE_FUNCTION
  wint_t ace_towlower (wint_t c);

  /// Converts a character to upper case (char version).
  ACE_NAMESPACE_INLINE_FUNCTION
  int ace_toupper (int c);

  /// Converts a character to upper case (wchar_t version).
  ACE_NAMESPACE_INLINE_FUNCTION
  wint_t ace_towupper (wint_t c);

  //@}

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_ctype.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/OS_NS_ctype_T.inl"

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_CTYPE_H */
