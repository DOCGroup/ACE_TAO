/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IORTable.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =========================================================================

#ifndef TAO_IORTABLE_H
#define TAO_IORTABLE_H
#include "ace/pre.h"

#include "iortable_export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IORTable_Export TAO_IORTable_Initializer
{
public:
  static int init (void);
  // Used to force the initialization of the ORB code.
};

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_IORTable_Initializer = &TAO_IORTable_Initializer::init;

#else

static int
TAO_Requires_IORTable_Initializer = TAO_IORTable_Initializer::init ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_IORTABLE_SAFE_INCLUDE
#include "IORTableC.h"
#undef TAO_IORTABLE_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_IORTABLE_H */
