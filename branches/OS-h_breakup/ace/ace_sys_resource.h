/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_resource.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_RESOURCE_H
#define ACE_ACE_SYS_RESOURCE_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <sys/resource.h>

# if defined (ACE_HAS_BROKEN_SETRLIMIT)
typedef struct rlimit ACE_SETRLIMIT_TYPE;
# else
typedef const struct rlimit ACE_SETRLIMIT_TYPE;
# endif /* ACE_HAS_BROKEN_SETRLIMIT */


#endif /* ACE_ACE_SYS_RESOURCE_H */
