/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_stdio.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_STDIO_H
# define ACE_ACE_STDIO_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_STDARG_H)
#   include /**/ <stdarg.h> // LynxOS requires this before stdio.h 
# endif /* !ACE_LACKS_STDARG_H */

# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#   include /**/ <cstdio>
# else
#   if !defined (ACE_LACKS_STDIO_H)
#     include /**/ <stdio.h>
#   endif /* !ACE_LACKS_STDIO_H */
# endif

# if defined (ACE_HAS_CHARPTR_SPRINTF)
#   define ACE_SPRINTF_ADAPTER(X) ::strlen (X)
# else
#   define ACE_SPRINTF_ADAPTER(X) X
# endif /* ACE_HAS_CHARPTR_SPRINTF */

// This is defined by XOPEN to be a minimum of 16.  POSIX.1g
// also defines this value.  platform-specific config.h can
// override this if need be.
# if !defined (IOV_MAX)
#   define IOV_MAX 16
# endif /* IOV_MAX */

# if !defined (ACE_IOV_MAX)
#   define ACE_IOV_MAX IOV_MAX
# endif /* ACE_IOV_MAX */

#if !defined (ACE_WIN32)
// Honestly don't know where these should go.
// Add some typedefs and macros to enhance Win32 conformance...
# if !defined (LPSECURITY_ATTRIBUTES)
#   define LPSECURITY_ATTRIBUTES int
# endif /* !defined LPSECURITY_ATTRIBUTES */
# if !defined (GENERIC_READ)
#   define GENERIC_READ 0
# endif /* !defined GENERIC_READ */
# if !defined (FILE_SHARE_READ)
#   define FILE_SHARE_READ 0
# endif /* !defined FILE_SHARE_READ */
# if !defined (OPEN_EXISTING)
#   define OPEN_EXISTING 0
# endif /* !defined OPEN_EXISTING */
# if !defined (FILE_ATTRIBUTE_NORMAL)
#   define FILE_ATTRIBUTE_NORMAL 0
# endif /* !defined FILE_ATTRIBUTE_NORMAL */
# if !defined (MAXIMUM_WAIT_OBJECTS)
#   define MAXIMUM_WAIT_OBJECTS 0
# endif /* !defined MAXIMUM_WAIT_OBJECTS */
# if !defined (FILE_FLAG_OVERLAPPED)
#   define FILE_FLAG_OVERLAPPED 0
# endif /* !defined FILE_FLAG_OVERLAPPED */
# if !defined (FILE_FLAG_SEQUENTIAL_SCAN)
#   define FILE_FLAG_SEQUENTIAL_SCAN 0
# endif   /* FILE_FLAG_SEQUENTIAL_SCAN */
#endif

# if !defined (ACE_OSTREAM_TYPE)
#   if defined (ACE_LACKS_IOSTREAM_TOTALLY)
#     define ACE_OSTREAM_TYPE FILE
#   else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#     define ACE_OSTREAM_TYPE ostream
#   endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
# endif /* ! ACE_OSTREAM_TYPE */

# if !defined (ACE_DEFAULT_LOG_STREAM)
#   if defined (ACE_LACKS_IOSTREAM_TOTALLY)
#     define ACE_DEFAULT_LOG_STREAM 0
#   else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#     define ACE_DEFAULT_LOG_STREAM (&cerr)
#   endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
# endif /* ! ACE_DEFAULT_LOG_STREAM */



// this is a nasty hack to get around problems with the
// pSOS definition of BUFSIZ as the config table entry
// (which is valued using the LC_BUFSIZ value anyway)
# if defined (ACE_PSOS)
#   if defined (BUFSIZ)
#     undef BUFSIZ
#   endif /* defined (BUFSIZ) */
#   define BUFSIZ LC_BUFSIZ
# endif /* defined (ACE_PSOS) */

# if defined (BUFSIZ)
#   define ACE_STREAMBUF_SIZE BUFSIZ
# else
#   define ACE_STREAMBUF_SIZE 1024
# endif /* BUFSIZ */

#endif /* ACE_ACE_STDIO_H */

