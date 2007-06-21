// -*- C++ -*-

// ===================================================================
/**
 *  @file   PolicyFactory_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_POLICY_FACTORY_REGISTRY_H
#define TAO_POLICY_FACTORY_REGISTRY_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/PolicyFactory_Registry_Adapter.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PolicyFactory_Registry
 *
 * @brief ORB-specific PortableInterceptor::PolicyFactory registry.
 *
 * ORB-specific registry that contains all portable interceptor
 * policy factories.
 */
class TAO_PolicyFactory_Registry
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
    PortableInterceptor::PolicyFactory_ptr policy_factory);

  /// Construct a policy of the given type with the information
  /// contained in the CORBA::Any @a value.
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any &value);

  /// Create an empty policy, usually to be filled in later by
  /// demarshaling.
  CORBA::Policy_ptr _create_policy (CORBA::PolicyType type);

  /// Check if a @c PolicyFactory corresponding to the given type,
  /// exists.
  bool factory_exists (CORBA::PolicyType & type) const;

private:

  /// The table that maps policy type to policy factory.
  TABLE factories_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_POLICY_FACTORY_REGISTRY_H */
