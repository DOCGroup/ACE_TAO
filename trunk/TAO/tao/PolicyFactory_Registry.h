// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     PolicyFactory_Registry.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_POLICY_FACTORY_REGISTRY_H
#define TAO_POLICY_FACTORY_REGISTRY_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "ace/Map_Manager.h"

class TAO_ORB_Core;

/// ORB-specific registry that contains all portable interceptor
/// policy factories.
class TAO_Export TAO_PolicyFactory_Registry
{
public:

  typedef ACE_Map_Iterator<
    CORBA::PolicyType,
    PortableInterceptor::PolicyFactory_ptr,
    ACE_Null_Mutex> ITERATOR;

  typedef ACE_Map_Manager<
    CORBA::PolicyType,
    PortableInterceptor::PolicyFactory_ptr,
    ACE_Null_Mutex> TABLE;
  ///< The type of table that maps policy type to policy factory.

  ///< NOTE: An ACE_Null_Mutex not used for any of these types since
  ///<       policy factories are only registered when
  ///<       CORBA::ORB_init() is called, at which a point a lock has
  ///<       already been acquired.  In short, the table is only
  ///<       modified during ORB bootstrap-time.

public:

  TAO_PolicyFactory_Registry (TAO_ORB_Core *orb_core);
  ///< Constructor

  ~TAO_PolicyFactory_Registry (void);
  ///< Destructor.  Releases duplicated PolicyFactory references.

  void register_policy_factory (
    CORBA::PolicyType type,
    PortableInterceptor::PolicyFactory_ptr policy_factory,
    CORBA::Environment &ACE_TRY_ENV);
  ///< Register a PolicyFactory with the underlying PolicyFactory
  ///< sequence.
  ///< This method should only be called during ORB initialization.

  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any &value,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::PolicyError));
  ///< Construct a policy of the given type with the information
  ///< contained in the CORBA::Any <value>.

private:

  TAO_ORB_Core *orb_core_;
  // @@ This should go away once the FT team is done switching to
  //    policy factories.  This attribute is only meant to ease that
  //    transition.

  TABLE factories_;
  ///< The table that maps policy type to policy factory.

};

#include "ace/post.h"

#endif /* TAO_POLICY_FACTORY_REGISTRY_H */
