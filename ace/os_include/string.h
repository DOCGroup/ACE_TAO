// -*- C++ -*-

//=============================================================================
/**
 *  @file    string.h
 *
 *  string operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_STRING_H
#define ACE_OS_INCLUDE_STRING_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/stddef.h"

#if !defined (ACE_LACKS_STRING_H)
# include /**/ <string.h>
#endif /* !ACE_LACKS_STRING_H */

#if defined (ACE_LACKS_STRTOK_R_PROTOTYPE) && !defined (_POSIX_SOURCE)
extern "C" char *strtok_r (char *s, const char *delim, char **save_ptr);
#endif  /* ACE_LACKS_STRTOK_R_PROTOTYPE */

#if defined (__BORLANDC__)
#  define _stricmp stricmp
#  define _strnicmp strnicmp
#endif /* __BORLANDC__ */

#if defined (ACE_HAS_CHARPTR_SPRINTF)
#  define ACE_SPRINTF_ADAPTER(X) ::strlen (X)
#else
#  define ACE_SPRINTF_ADAPTER(X) X
#endif /* ACE_HAS_CHARPTR_SPRINTF */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_STRING_H */
