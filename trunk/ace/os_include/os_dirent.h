// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_dirent.h
 *
 *  format of directory entries
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_DIRENT_H
#define ACE_OS_INCLUDE_OS_DIRENT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"
#include "ace/os_include/os_limits.h"

#if !defined (ACE_LACKS_DIRENT_H)
# include /**/ <dirent.h>
#endif /* !ACE_LACKS_DIRENT_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if !defined (MAXNAMLEN)
#  define MAXNAMLEN NAME_MAX
#endif /* !MAXNAMLEN */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_DIRENT_H */
