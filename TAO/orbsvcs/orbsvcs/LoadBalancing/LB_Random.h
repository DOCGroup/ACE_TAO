// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_Random.h
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


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_Random
 *
 * @brief "Random" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at a random location.
 */
class TAO_LB_Random
  : public virtual POA_CosLoadBalancing::Strategy
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
  char * name (void) override;

  CosLoadBalancing::Properties * get_properties () override;

  void push_loads (
      const PortableGroup::Location & the_location,
      const CosLoadBalancing::LoadList & loads) override;

  CosLoadBalancing::LoadList * get_loads (
      CosLoadBalancing::LoadManager_ptr load_manager,
      const PortableGroup::Location & the_location) override;

  CORBA::Object_ptr next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager) override;

  void analyze_loads (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager) override;
  //@}

  /// Returns the default POA for this servant.
  PortableServer::POA_ptr _default_POA (
    ) override;

  /// TAO-specific underlying implementation of this Random load
  /// balancing strategy's next_member() method.
  static CORBA::Object_ptr _tao_next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager,
      const PortableGroup::Locations & locations);

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

TAO_END_VERSIONED_NAMESPACE_DECL


#include /**/ "ace/post.h"

#endif  /* LB_RANDOM_H */
