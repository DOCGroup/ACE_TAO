// -*- C++ -*-

//=============================================================================
/**
 *  @file    pthread.h
 *
 *  threads
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_PTHREAD_H
#define ACE_OS_INCLUDE_PTHREAD_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_PTHREAD_H)
# include /**/ <pthread.h>
#endif /* !ACE_LACKS_PTHREAD_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_PTHREAD_H */
