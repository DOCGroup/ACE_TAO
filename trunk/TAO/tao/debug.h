// $Id$

// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    debug.h
//
// = DESCRIPTION
//     debug/trace support.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#ifndef TAO_DEBUG_H
#define TAO_DEBUG_H

#include "tao/corbafwd.h"

// These are global to simplify is use by other code, very much in
// particular by getopt and related argument-parsing code
//
// THREADING NOTE: don't set them except in an unthreaded environment
// such as process initialization.  They're treated as immutable.

// 0 to ??; higher == more
extern TAO_Export u_int TAO_debug_level;

// debug messages on (1) or off (0)
extern TAO_Export u_int TAO_orbdebug;

// set by getopt
extern TAO_Export char *TAO_debug_filter;

#endif /* TAO_DEBUG_H */
