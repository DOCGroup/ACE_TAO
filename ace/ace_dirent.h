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

# include /**/ <dirent.h>

# if !defined (MAXNAMLEN)
#   if defined (ACE_PSOS) || defined (VXWORKS)
#     define MAXNAMLEN 255
#   elif defined (ACE_WIN32)
#     define MAXNAMLEN _MAX_FNAME
#   endif /* ACE_WIN32 */
// This is probably wrong, but...
#   define MAXNAMLEN 255 
# endif /* !MAXNAMLEN */

#endif /* ACE_ACE_DIRENT_H */
