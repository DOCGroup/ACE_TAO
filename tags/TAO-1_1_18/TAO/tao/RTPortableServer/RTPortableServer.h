/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    RTPortableserver.h
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_RT_PORTABLESERVER_H
#define TAO_RT_PORTABLESERVER_H
#include "ace/pre.h"

#include "rtportableserver_export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTPortableServer_Export TAO_RT_POA_Initializer
{
public:
  static int init (void);
  // Used to force the initialization of the ORB code.
};

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_RT_POA_Initializer = &TAO_RT_POA_Initializer::init;

#else

static int
TAO_Requires_RT_POA_Initializer  = TAO_RT_POA_Initializer::init ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_RT_PORTABLESERVER_SAFE_INCLUDE
#include "RTPortableServerC.h"
#undef TAO_RT_PORTABLESERVER_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_RT_PORTABLESERVER_H */
