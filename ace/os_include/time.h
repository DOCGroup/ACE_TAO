// -*- C++ -*-

//=============================================================================
/**
 *  @file    time.h
 *
 *  time types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_TIME_H
#define ACE_OS_INCLUDE_TIME_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @todo should we include anything from signal.h?
#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_TIME_H)
# include /**/ <time.h>
#endif /* !ACE_LACKS_TIME_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_TIME_H */
