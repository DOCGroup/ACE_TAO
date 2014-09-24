// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Implementation.h
 *
 *  $Id$
 *
 *  Header file for class TAO_DynamicImplementation.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNAMIC_IMPLEMENTATION_H
#define TAO_DYNAMIC_IMPLEMENTATION_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  typedef char *RepositoryId;
  typedef String_var RepositoryId_var;

  class ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;
}

/**
 * @class TAO_DynamicImplementation
 *
 * @brief Base class for DSI.
 *
 * It is expected that the <invoke> and <_primary_interface>
 * methods will be only invoked by the POA in the context of
 * serving a CORBA request. Invoking this method in other
 * circumstances may lead to unpredictable results.
 */
class TAO_DynamicInterface_Export TAO_DynamicImplementation
  : public virtual TAO_ServantBase
{
public:
  /// The invoke() method receives requests issued to any CORBA object
  /// incarnated by the DSI servant and performs the processing
  /// necessary to execute the request.
  virtual void invoke (CORBA::ServerRequest_ptr request) = 0;

  /// The _primary_interface() method receives an ObjectId value and a
  /// POA_ptr as input parameters and returns a valid RepositoryId
  /// representing the most-derived interface for that oid.
  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa) = 0;

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char *logical_type_id);

  /// Returns a CORBA::Object_ptr for the target object.
  CORBA::Object_ptr _this (void);

  /// Query the Interface Repository for the interface definition.
  virtual CORBA::InterfaceDef_ptr _get_interface (void);

protected:

  /// Return 0.  Should never be used.
  virtual const char *_interface_repository_id (void) const;

  /// Simply returns "this"
  virtual void *_downcast (const char *repository_id);

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (void);

  /// Turns around and calls invoke.
  virtual void _dispatch (
    TAO_ServerRequest &request,
    TAO::Portable_Server::Servant_Upcall *context);

private:
  /// Encapsulates code common to _is_a(), _get_interface() and _create_stub().
  CORBA::RepositoryId get_id_from_primary_interface (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_DYNAMIC_IMPLEMENTATION_H */
