// -*- C++ -*-

//=============================================================================
/**
 *  @file    wordexp.h
 *
 *  word-expansion types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_WORDEXP_H
#define ACE_OS_INCLUDE_WORDEXP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @todo make sure that size_t is in stddef.h and sys/types.h includes stddef.h
#include "ace/os_include/stddef.h" // size_t

#if !defined (ACE_LACKS_WORDEXP_H)
# include /**/ <wordexp.h>
#endif /* !ACE_LACKS_WORDEXP_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_WORDEXP_H */
