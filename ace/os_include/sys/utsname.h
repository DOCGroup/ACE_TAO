/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    utsname.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_UTSNAME_H
# define ACE_OS_INCLUDE_SYS_UTSNAME_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SYS_UTSNAME_H)
#   include /**/ <sys/utsname.h>
# endif /* !ACE_LACKS_SYS_UTSNAME_H */


# if defined (ACE_LACKS_UTSNAME_T)
#   if !defined (SYS_NMLN)
#     define SYS_NMLN 257
#   endif /* SYS_NMLN */
#   if !defined (_SYS_NMLN)
#     define _SYS_NMLN SYS_NMLN
#   endif /* _SYS_NMLN */
struct ACE_utsname
{
  ACE_TCHAR sysname[_SYS_NMLN];
  ACE_TCHAR nodename[_SYS_NMLN];
  ACE_TCHAR release[_SYS_NMLN];
  ACE_TCHAR version[_SYS_NMLN];
  ACE_TCHAR machine[_SYS_NMLN];
};
# else
typedef struct utsname ACE_utsname;
# endif /* ACE_LACKS_UTSNAME_T */

#endif /* ACE_OS_INCLUDE_SYS_UTSNAME_H */
