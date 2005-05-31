// -*- C++ -*-

// ===================================================================
/**
 *  @file   PolicyFactory_Registry_Adapter.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_POLICY_FACTORY_REGISTRY_ADAPTER_H
#define TAO_POLICY_FACTORY_REGISTRY_ADAPTER_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"

#include "Basic_Types.h"

//-- Forward Declarations--
namespace CORBA
{
  typedef ULong PolicyType;

  class Policy;
  typedef Policy *Policy_ptr;

  class Any;

  class Environment;
}

namespace PortableInterceptor
{
  class PolicyFactory;
  typedef PolicyFactory *PolicyFactory_ptr;
}

namespace TAO
{
  /**
   * @class PolicyFactory_Registry_Adapter
   *
   * @brief ORB-specific PortableInterceptor::PolicyFactory registry.
   *
   * Class that offers an interface to the ORB to load and manipulate
   * PolicyFactory_Registry
   */
  class TAO_Export PolicyFactory_Registry_Adapter
    : public ACE_Service_Object
  {
  public:
    /// Register a PolicyFactory with the underlying PolicyFactory
    /// sequence.  This method should only be called during ORB
    /// initialization.
    virtual void register_policy_factory (
      CORBA::PolicyType type,
      PortableInterceptor::PolicyFactory_ptr policy_factory
      ACE_ENV_ARG_DECL) = 0;

    /// Construct a policy of the given type with the information
    /// contained in the CORBA::Any @a value.
    virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                             const CORBA::Any &value
                                             ACE_ENV_ARG_DECL) = 0;

    /// Create an empty policy, usually to be filled in later by
    /// demarshaling.
    virtual CORBA::Policy_ptr _create_policy (CORBA::PolicyType type
                                              ACE_ENV_ARG_DECL) = 0;

    /// Check if a @c PolicyFactory corresponding to the given type,
    /// exists.
    virtual bool factory_exists (CORBA::PolicyType & type) const = 0;
  };
}

#include /**/ "ace/post.h"

#endif  /* TAO_POLICY_FACTORY_REGISTRY_ADAPTER_H */
