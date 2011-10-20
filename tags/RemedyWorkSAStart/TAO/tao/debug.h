// -*- C++ -*-

//=============================================================================
/**
 *  @file debug.h
 *
 *  $Id$
 *
 *  @author DOC Group
 */
//=============================================================================

#ifndef TAO_DEBUG_H
#define TAO_DEBUG_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// These are global to simplify is use by other code, very much in
// particular by getopt and related argument-parsing code
//
// THREADING NOTE: don't set them except in an unthreaded environment
// such as process initialization.  They're treated as immutable.

// 0 to ??; higher == more
extern TAO_Export unsigned int TAO_debug_level;

// debug messages on (1) or off (0)
extern TAO_Export unsigned int TAO_orbdebug;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DEBUG_H */
