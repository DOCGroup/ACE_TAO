// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_Random.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_RANDOM_H
#define LB_RANDOM_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingS.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_LB_Random
 *
 * @brief "Random" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at a random location.
 */
class TAO_LB_Random
  : public virtual POA_CosLoadBalancing::Strategy,
    public virtual PortableServer::RefCountServantBase
{
public:

  /// Constructor.
  /**
   * Seeds the OS' random number generator.
   */
  TAO_LB_Random (PortableServer::POA_ptr poa);

  /**
   * @name CosLoadBalancing::Strategy methods
   *
   * Methods required by the CosLoadBalancing::Strategy interface.
   */
  //@{
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CosLoadBalancing::Properties * get_properties (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void push_loads (
      const PortableGroup::Location & the_location,
      const CosLoadBalancing::LoadList & loads
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosLoadBalancing::StrategyNotAdaptive));

  virtual CosLoadBalancing::LoadList * get_loads (
      CosLoadBalancing::LoadManager_ptr load_manager,
      const PortableGroup::Location & the_location
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosLoadBalancing::LocationNotFound));

  virtual CORBA::Object_ptr next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  virtual void analyze_loads (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// TAO-specific underlying implementation of this Random load
  /// balancing strategy's next_member() method.
  static CORBA::Object_ptr _tao_next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager,
      const PortableGroup::Locations & locations
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  /// Initialize the random load balancing strategy.
  /**
   * If supported by the platform, initialization code will only run
   * once for a given process.
   */
  static void init (void);

private:

  /// This servant's default POA.
  PortableServer::POA_var poa_;

};


#include /**/ "ace/post.h"

#endif  /* LB_RANDOM_H */
