// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/orbsvcs/IFR_Service
//
// = FILENAME
//     TCF_Loader.h
//
// = AUTHOR
//     Jeff Parsons
//
// ============================================================================

#ifndef TAO_OBJECT_LOADER_TCF_LOADER_H
#define TAO_OBJECT_LOADER_TCF_LOADER_H
#include "ace/pre.h"

#include "TCF_Export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TCF_Export TCF_Loader : public TAO_Object_Loader
{
  // = TITLE
  //     TCF_Loader
  //
  // = DESCRIPTION
  //     Create an instance of TypeCodeFactory and return
  //     it to the caller.
  //
public:
  TCF_Loader (void);
  // Constructor.

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv [],
                                           CORBA::Environment &)
    ACE_THROW_SPEC (());
  // Overload the base class method to create a new instance
  // of a TypeCodeFactory object.

  static int Initializer (void);
  // Used to force the initialization of the ORB code.
};

ACE_STATIC_SVC_DECLARE (TCF_Loader)
ACE_FACTORY_DECLARE (TCF, TCF_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_TCF_Initializer = &TCF_Loader::Initializer;

#else

static int
TAO_Requires_TCF_Initializer = TCF_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_TYPECODEFACTORY_SAFE_INCLUDE
#include "TypeCodeFactoryC.h"
#undef TAO_TYPECODEFACTORY_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_OBJECT_LOADER_TCF_LOADER_H */
