/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Windows
// 9x, Windows NT 3.51, and Windows NT 4.0 platforms and supports a
// variety of compilers.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

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

#endif /* ACE_CONFIG_H */
