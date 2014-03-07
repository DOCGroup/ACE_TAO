// -*- C++ -*-

//=============================================================================
/**
 *  @file    Adapter_Registry.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================

#ifndef TAO_ADAPTER_REGISTRY_H
#define TAO_ADAPTER_REGISTRY_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "tao/Pseudo_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
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
class TAO_Adapter;

class TAO_Export TAO_Adapter_Registry
{
public:
  explicit TAO_Adapter_Registry (TAO_ORB_Core *orb_core);

  /// Close the
  ~TAO_Adapter_Registry (void);

  /**
   * Close each of of the Adapters and then cleanup the Registry.
   * It is possible that an Adapter will reject a close() call if it
   * is invoked in an inappropriate context (think shutting down the
   * POA while performing an upcall).
   */
  void close (int wait_for_completion);

  /**
   * Verify if the close() call can be invoked in the current
   * context for *all* adapters.
   * Raise the right exception if not.
   */
  void check_close (int wait_for_completion);

  /// Insert a new adapter into the registry.
  void insert (TAO_Adapter *adapter);

  /**
   * Dispatch the request to all the adapters.
   * It tries the adapters ordered by priority, stopping when the
   * adapter returns a status different from DS_MISMATCHED_KEY
   */
  void dispatch (TAO::ObjectKey &key,
                 TAO_ServerRequest &request,
                 CORBA::Object_out forward_to);

  /// Create a collocated object using the given profile and stub.
  CORBA::Object_ptr create_collocated_object (TAO_Stub *, const TAO_MProfile &);

  /// Initialize a collocated object using the given stub and object
  /// pointer for lazily evaluated object references.
  CORBA::Long initialize_collocated_object (TAO_Stub *);

  /// Fetch the adapter named @a name
  TAO_Adapter *find_adapter (const char *name) const;

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_Adapter_Registry (const TAO_Adapter_Registry &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Adapter_Registry &operator= (const TAO_Adapter_Registry &))

  /// The ORB Core
  TAO_ORB_Core *orb_core_;

  /**
   * @name A simple array of adapters.
   */
  //@{
  size_t adapters_capacity_;
  size_t adapters_count_;
  TAO_Adapter **adapters_;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ADAPTER_REGISTRY_H */
