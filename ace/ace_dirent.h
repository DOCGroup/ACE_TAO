/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_dirent.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_DIRENT_H
# define ACE_ACE_DIRENT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ace_sys_types.h"
#include "ace/ace_unistd.h"

# if !defined (ACE_LACKS_DIRENT_H)
#   include /**/ <dirent.h>
# endif /* ACE_LACKS_DIRENT_H */

# if !defined (MAXNAMLEN)
#   if !defined (ACE_MAXNAMLEN)
#     define ACE_MAXNAMLEN 255
#   endif /* !ACE_MAXNAMELEN */
#   define MAXNAMLEN ACE_MAXNAMLEN
# endif /* !MAXNAMLEN */

#endif /* ACE_ACE_DIRENT_H */
