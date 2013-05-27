// -*- C++ -*-

//=============================================================================
/**
 *  @file    Adapter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================

#ifndef TAO_ADAPTER_H
#define TAO_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/CORBA_methods.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Pseudo_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object * Object_ptr;

  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object> Object_out;
}

namespace TAO
{
  class ObjectKey;
}

class TAO_ORB_Core;
class TAO_Stub;
class TAO_MProfile;
class TAO_ServerRequest;

class TAO_Export TAO_Adapter
{
public:
  virtual ~TAO_Adapter (void);

  /// Initialize the Adapter
  virtual void open (void) = 0;

  /// The ORB is shutting down, destroy any resources attached to this
  /// adapter.
  virtual void close (int wait_for_completion) = 0;

  /// Check if the adapter can be closed in the current context, raise
  /// an exception if not.
  virtual void check_close (int wait_for_completion) = 0;

  /**
   * Return the priority assigned to this adapter.
   * Adapters at higher priority are used first, the first adapter
   * that matches a key is used to dispatch a request.
   */
  virtual int priority (void) const = 0;

  /// Return the status....
  virtual int dispatch (TAO::ObjectKey &key,
                        TAO_ServerRequest &request,
                        CORBA::Object_out forward_to) = 0;

  enum {
    /// The operation was successfully dispatched, an exception may
    /// have been raised, but that is a correct execution too.
    DS_OK,

    /// There was a problem in dispatching the operation.
    DS_FAILED,

    /// The key is not in the right format for this Adapter, try the
    /// next one.
    DS_MISMATCHED_KEY,

    /// Forward the request to another object reference, this decouples
    /// the ORB from the PortableServer::ForwardRequest exception
    DS_FORWARD,

    /// The request will be forwarded, but using an async call to locate
    /// the target. Some other entity will actually complete the forward
    /// or fail.
    DS_DEFERRED_FORWARD
  };

  /// Return the name, i.e. the object id used to resolve it in the
  /// ORB.
  virtual const char *name (void) const = 0;

  /**
   * Return the root of the Object Adapter.
   * Each adapter defines its own IDL interface accessed through the
   * method above.
   */
  virtual CORBA::Object_ptr root (void) = 0;

  /// Create a collocated object using the given profile and stub.
  virtual CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                                      const TAO_MProfile &) = 0;

  /// Initialize a collocated object using the given stub
  /// pointer for lazily evaluated object references.
  virtual CORBA::Long initialize_collocated_object (TAO_Stub *) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ADAPTER_H */
