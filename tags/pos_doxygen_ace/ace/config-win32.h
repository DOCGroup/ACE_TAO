/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Windows
// 9x, Windows NT 3.51, and Windows NT 4.0 platforms and supports a
// variety of compilers.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

// If you wish to build ACE using PACE, you must uncomment
// the following lines or include the following syntax in your
// config file #included before this one!
//# if !defined (ACE_HAS_PACE)
//#  define ACE_HAS_PACE
//# endif // ACE_HAS_PACE

# if defined (ACE_HAS_PACE)
#  ifndef PACE_HAS_ALL_POSIX_FUNCS
#   define PACE_HAS_ALL_POSIX_FUNCS 1
#  endif /* PACE_HAS_ALL_POSIX_FUNCS */
# endif /* ACE_HAS_PACE */

#if defined (ACE_HAS_WINCE) || defined (UNDER_CE)
# include "ace/config-WinCE.h"
#endif /* ACE_HAS_WINCE */

#if defined (_MSC_VER)
# include "ace/config-win32-msvc.h"
#elif defined (__BORLANDC__)
# include "ace/config-win32-borland.h"
#elif defined (__IBMCPP__)
# include "ace/config-win32-visualage.h"
#else
# error "Compiler does not seem to be supported"
#endif /* _MSC_VER */

#if !defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#  define ACE_ENDTHREADEX(STATUS) ::_endthread ()
#elif defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#  define ACE_ENDTHREADEX(STATUS) ::_endthread ()
#elif defined (ACE_HAS_WINCE) && defined (UNDER_CE) && (UNDER_CE >= 211)
#  define ACE_ENDTHREADEX(STATUS) ExitThread ((DWORD) STATUS)
#else
#  define ACE_ENDTHREADEX(STATUS) ::_endthreadex ((DWORD) STATUS)
#endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
