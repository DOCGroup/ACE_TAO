/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableGroup_Loader.h
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_PORTABLEGROUP_LOADER_H
#define TAO_PORTABLEGROUP_LOADER_H
#include "ace/pre.h"

#include "portablegroup_export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PortableGroup_Export TAO_PGA_Loader : public TAO_Object_Loader
{
public:
  TAO_PGA_Loader (void);
  // Constructor

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv [],
                                           CORBA::Environment &)
    ACE_THROW_SPEC (());
  // Creates the RootPGA object and returns it.

  static int Initializer (void);
  // Used to force the initialization of the ORB code.
};

ACE_STATIC_SVC_DECLARE (TAO_PGA_Loader)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_PGA_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_IORManip_Initializer = &TAO_PGA_Loader::Initializer;

#else

static int
TAO_Requires_IORManip_Initializer = TAO_PGA_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

//#define TAO_PORTABLEGROUP_SAFE_INCLUDE
//#include "IORC.h"
//#undef TAO_IORMANIP_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_LOADER_H */
