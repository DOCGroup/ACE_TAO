// -*- C++ -*-

//=============================================================================
/**
 *  @file    ctype.h
 *
 *  character types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_CTYPE_H
#define ACE_OS_INCLUDE_CTYPE_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LACKS_CTYPE_H)
# include /**/ <ctype.h>
#endif /* !ACE_LACKS_CTYPE_H */

// @todo move the is* and is* emulation methods in ACE_OS here
// and let ACE_OS just call them.

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_CTYPE_H */
