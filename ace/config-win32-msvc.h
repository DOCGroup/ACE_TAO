//=============================================================================
/**
 *  @file   config-win32-msvc.h
 *
 *  $Id$
 *
 *  @brief  Microsoft Visual C++ configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++ versions
 *  5.0, 6.0, and 7.0 (.NET)
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_H
#define ACE_CONFIG_WIN32_MSVC_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_CC_NAME ACE_LIB_TEXT ("Visual C++")
#define ACE_CC_PREPROCESSOR "CL.EXE"
#define ACE_CC_PREPROCESSOR_ARGS "-nologo -E"

#define ACE_CC_MAJOR_VERSION (_MSC_VER / 100 - 6)
#define ACE_CC_MINOR_VERSION (_MSC_VER % 100)
#define ACE_CC_BETA_VERSION (0)

// Support "decorated" library names for MSVC 6 or better.
#if (_MSC_VER >= 1200)
# if !defined (ACE_LD_DECORATOR_STR)
#  if defined (_DEBUG)
#   if ACE_HAS_MFC == 1
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("mfcd")
#   else
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("d")
#   endif  /* ACE_HAS_MFC */
#  else  /* _NDEBUG */
#   if ACE_HAS_MFC == 1
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("mfc")
#   endif  /* ACE_HAS_MFC */
#  endif  /* _DEBUG */
# endif  /* ACE_LD_DECORATOR_STR */
#endif  /* _MSC_VER >= 1200 */

#if (_MSC_VER >= 1300)
# include "ace/config-win32-msvc-7.h"
#elif (_MSC_VER >= 1200)
# include "ace/config-win32-msvc-6.h"
#elif (_MSC_VER >= 1100)
# include "ace/config-win32-msvc-5.h"
#else
# error This version of Microsoft Visual C++ not supported.
#endif

#include <malloc.h>
#define ACE_HAS_ALLOCA 1

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_H */
