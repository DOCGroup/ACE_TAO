/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Method_Object.h
//
// = DESCRIPTION
//    This file just #includes "ace/Method_Request.h" and is just here
//    for backwards compatibility with earlier versions of ACE.
//    Please don't use it directly since it may go away at some point.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_METHOD_OBJECT_H
#include "ace/pre.h"
#define ACE_METHOD_OBJECT_H

#include "ace/Method_Request.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Maintain backwards compatibility so that Steve Huston doesn't go
// postal... ;-)
typedef ACE_Method_Request ACE_Method_Object;

#include "ace/post.h"
#endif /* ACE_METHOD_OBJECT_H */
