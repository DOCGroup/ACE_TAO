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

#include "tao/Object_Loader.h"
#include "TCF_Export.h"

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
  // ctor

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Overload the base class method to create a new instance
  // of a TypeCodeFactory object.
};

// ACE_STATIC_SVC_DECLARE (TCF_Loader)
ACE_FACTORY_DECLARE (TCF, TCF_Loader)

#endif /* TAO_OBJECT_LOADER_TCF_LOADER_H */
