// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    DynamicInterface
//
// = FILENAME
//    Dynamic_Implementation.h
//
// = DESCRIPTION
//    Header file for class TAO_DynamicImplementation.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DYNAMIC_IMPLEMENTATION_H
#define TAO_DYNAMIC_IMPLEMENTATION_H
#include "ace/pre.h"

#include "tao/PortableServer/Servant_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dynamicinterface_export.h"

class TAO_DynamicInterface_Export TAO_DynamicImplementation
  : public virtual TAO_ServantBase
{
  // = TITLE
  //    Base class for DSI.
  //
  // = DESCRIPTION
  //    It is expected that the <invoke> and <_primary_interface>
  //    methods will be only invoked by the POA in the context of
  //    serving a CORBA request. Invoking this method in other
  //    circumstances may lead to unpredictable results.
public:
  virtual void invoke (CORBA::ServerRequest_ptr request
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;
  // The invoke() method receives requests issued to any CORBA object
  // incarnated by the DSI servant and performs the processing
  // necessary to execute the request.

  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa
      ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  // The _primary_interface() method receives an ObjectId value and a
  // POA_ptr as input parameters and returns a valid RepositoryId
  // representing the most-derived interface for that oid.

  CORBA::Object_ptr _this (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Returns a CORBA::Object_ptr for the target object.

protected:

  virtual const char *_interface_repository_id (void) const;
  // Return 0.  Should never be used.

  virtual void *_downcast (const char *repository_id);
  // Simply returns "this"

  virtual TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // This is an auxiliary method for _this() and _narrow().

  virtual void _dispatch (TAO_ServerRequest &request,
                          void *context
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Turns around and calls invoke.
};

#include "ace/post.h"
#endif /* TAO_DYNAMIC_IMPLEMENTATION_H */
