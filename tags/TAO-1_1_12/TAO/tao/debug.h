
// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    debug.h
 *
 *  $Id$
 *
 *   debug/trace support.
 *
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 */
//=============================================================================


#ifndef TAO_DEBUG_H
#define TAO_DEBUG_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// These are global to simplify is use by other code, very much in
// particular by getopt and related argument-parsing code
//
// THREADING NOTE: don't set them except in an unthreaded environment
// such as process initialization.  They're treated as immutable.

// 0 to ??; higher == more
extern TAO_Export u_int TAO_debug_level;

// debug messages on (1) or off (0)
extern TAO_Export u_int TAO_orbdebug;

#include "ace/post.h"
#endif /* TAO_DEBUG_H */
