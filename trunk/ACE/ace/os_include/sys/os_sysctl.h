// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_sysctl.h
 *
 *  declarations for sysctl
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_SYSCTL_H
#define ACE_OS_INCLUDE_SYS_OS_SYSCTL_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LACKS_SYS_SYSCTL_H)
#  include /**/ <sys/sysctl.h>
#endif /* !ACE_LACKS_SYS_SYSCTL_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_SYSCTL_H */
