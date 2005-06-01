// -*- C++ -*-

// ===================================================================
/**
 *  @file   PolicyFactory_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_POLICY_FACTORY_REGISTRY_H
#define TAO_POLICY_FACTORY_REGISTRY_H

#include /**/ "ace/pre.h"

#include "pi_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/Basic_Types.h"

#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/CORBA_macros.h"
#include "tao/PolicyFactory_Registry_Adapter.h"

/**
 * @class TAO_PolicyFactory_Registry
 *
 * @brief ORB-specific PortableInterceptor::PolicyFactory registry.
 *
 * ORB-specific registry that contains all portable interceptor
 * policy factories.
 */
class TAO_PI_CLIENT_Export TAO_PolicyFactory_Registry
  : public TAO::PolicyFactory_Registry_Adapter
{
public:

  /**
   * The type of table that maps policy type to policy factory.
   *
   * @note
   * An ACE_Null_Mutex is used for this type since policy factories
   * are only registered when CORBA::ORB_init() is called, at which a
   * point a lock has already been acquired.  In short, the table is
   * only modified during ORB bootstrap-time.
   */
  typedef ACE_Map_Manager<CORBA::PolicyType,
                          PortableInterceptor::PolicyFactory_ptr,
                          ACE_Null_Mutex>
    TABLE;

public:

  /// Constructor
  TAO_PolicyFactory_Registry (void);

  /// Destructor.  Releases duplicated PolicyFactory references.
  ~TAO_PolicyFactory_Registry (void);

  /// Register a PolicyFactory with the underlying PolicyFactory
  /// sequence.  This method should only be called during ORB
  /// initialization.
  void register_policy_factory (
    CORBA::PolicyType type,
    PortableInterceptor::PolicyFactory_ptr policy_factory
    ACE_ENV_ARG_DECL);

  /// Construct a policy of the given type with the information
  /// contained in the CORBA::Any @a value.
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any &value
                                   ACE_ENV_ARG_DECL);

  /// Create an empty policy, usually to be filled in later by
  /// demarshaling.
  CORBA::Policy_ptr _create_policy (CORBA::PolicyType type
                                    ACE_ENV_ARG_DECL);

  /// Check if a @c PolicyFactory corresponding to the given type,
  /// exists.
  bool factory_exists (CORBA::PolicyType & type) const;

private:

  /// The table that maps policy type to policy factory.
  TABLE factories_;

};

ACE_STATIC_SVC_DECLARE (TAO_PolicyFactory_Registry)
ACE_FACTORY_DECLARE (TAO_PI_CLIENT, TAO_PolicyFactory_Registry)

#include /**/ "ace/post.h"

#endif  /* TAO_POLICY_FACTORY_REGISTRY_H */
