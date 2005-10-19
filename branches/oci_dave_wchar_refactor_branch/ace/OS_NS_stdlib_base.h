/* -*- C++ -*- */
// $Id$
#ifndef OS_NS_STDLIB_BASE_H
#define OS_NS_STDLIB_BASE_H

#include <stdlib.h>
// Standard library includes for wide functions declared in ace_wchar.h
// Do not include any ACE header files because this file is included from
// ace_wchar.h which is included at the config.h level.

namespace ACE_OS
{

/// Finds the contents of the strings (emulated version).
template <typename DCHAR, typename SCHAR> inline
size_t string_copy (DCHAR* s, const SCHAR* t, size_t len);

/// Finds the contents of the strings (wide to char version).
#if !defined(ACE_LACKS_WCSTOMBS)
template <> inline
size_t string_copy (char* s, const wchar_t* t, size_t len);
#endif

/// Finds the contents of the strings (char to wide version).
#if !defined(ACE_LACKS_MBSTOWCS)
template <> inline
size_t string_copy (wchar_t* s, const char* t, size_t len);
#endif

inline size_t mbstowcs (wchar_t* dest, const char* src, size_t size)
  { return ACE_OS::string_copy( dest, src, size ); }

inline size_t wcstombs (char* dest, const wchar_t* src, size_t size)
  { return ACE_OS::string_copy( dest, src, size ); }

}

#include "ace/OS_NS_stdlib_base.inl"

#endif
