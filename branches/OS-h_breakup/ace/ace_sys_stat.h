/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_stat.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_STAT_H
#define ACE_ACE_SYS_STAT_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <sys/stat.h>


# if defined (ACE_LACKS_MODE_MASKS)
// MODE MASKS

// the following macros are for POSIX conformance.

#   if !defined (ACE_HAS_USER_MODE_MASKS)
#     define S_IRWXU 00700         /* read, write, execute: owner. */
#     define S_IRUSR 00400         /* read permission: owner. */
#     define S_IWUSR 00200         /* write permission: owner. */
#     define S_IXUSR 00100         /* execute permission: owner. */
#   endif /* ACE_HAS_USER_MODE_MASKS */
#   define S_IRWXG 00070           /* read, write, execute: group. */
#   define S_IRGRP 00040           /* read permission: group. */
#   define S_IWGRP 00020           /* write permission: group. */
#   define S_IXGRP 00010           /* execute permission: group. */
#   define S_IRWXO 00007           /* read, write, execute: other. */
#   define S_IROTH 00004           /* read permission: other. */
#   define S_IWOTH 00002           /* write permission: other. */
#   define S_IXOTH 00001           /* execute permission: other. */

# endif /* ACE_LACKS_MODE_MASKS */




#endif /* ACE_ACE_SYS_STAT_H */
