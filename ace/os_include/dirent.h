/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    dirent.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_DIRENT_H
# define ACE_OS_INCLUDE_DIRENT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"
#include "ace/os_include/unistd.h"

# if !defined (ACE_LACKS_DIRENT_H)
#   include /**/ <dirent.h>
# endif /* ACE_LACKS_DIRENT_H */

# if !defined (MAXNAMLEN)
#   if !defined (ACE_MAXNAMLEN)
#     define ACE_MAXNAMLEN 255
#   endif /* !ACE_MAXNAMELEN */
#   define MAXNAMLEN ACE_MAXNAMLEN
# endif /* !MAXNAMLEN */

#endif /* ACE_OS_INCLUDE_DIRENT_H */
