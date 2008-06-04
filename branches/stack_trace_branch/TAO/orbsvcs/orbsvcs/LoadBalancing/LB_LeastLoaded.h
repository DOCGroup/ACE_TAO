// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_LeastLoaded.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_LEAST_LOADED_H
#define LB_LEAST_LOADED_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LB_LoadMap.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingS.h"

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_LB
{
  /**
   * @name Default LeastLoaded strategy parameters.
   *
   * Defaults defined by the Load Balancing specification.
   */
  //@{
  const CORBA::Float LL_DEFAULT_CRITICAL_THRESHOLD = 0;
  const CORBA::Float LL_DEFAULT_REJECT_THRESHOLD = 0;
  const CORBA::Float LL_DEFAULT_TOLERANCE = 1;
  const CORBA::Float LL_DEFAULT_DAMPENING = 0;
  const CORBA::Float LL_DEFAULT_PER_BALANCE_LOAD = 0;
  //@}

  /**
   * @name TAO-specific LeastLoaded strategy parameters.
   *
   * Parameters internal to TAO's LeastLoaded strategy
   * implementation.
   */
  //@{
  /// Percentage difference between two load values that determines
  /// whether the loads are considered equivalent.
  /**
   * If the percent difference between two loads, i.e.:
   *   (Old Load - New Load) / New Load
   * is less than or equal to this value, the two loads will be
   * considered equivalent.  In such a case, an object group member
   * residing at the location corresponding to one of the two loads
   * will be selected at random.
   */
  const CORBA::Float LL_DEFAULT_LOAD_PERCENT_DIFF_CUTOFF = 0.01f;  // 1%
  //@}
}

/**
 * @class TAO_LB_LeastLoaded
 *
 * @brief "Least loaded" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at a location with the smallest load.
 */
class TAO_LB_LeastLoaded
  : public virtual POA_CosLoadBalancing::Strategy
{
public:

  /// Constructor.
  TAO_LB_LeastLoaded (PortableServer::POA_ptr poa);

  /**
   * @name CosLoadBalancing::Strategy methods
   *
   * Methods required by the CosLoadBalancing::Strategy interface.
   */
  //@{
  virtual char * name (void);

  virtual CosLoadBalancing::Properties * get_properties ();

  virtual void push_loads (
      const PortableGroup::Location & the_location,
      const CosLoadBalancing::LoadList & loads);

  virtual CosLoadBalancing::LoadList * get_loads (
      CosLoadBalancing::LoadManager_ptr load_manager,
      const PortableGroup::Location & the_location);

  virtual CORBA::Object_ptr next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager);

  virtual void analyze_loads (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager);
  //@}

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (
    );

  /// Initialize the LeastLoaded instance with the given properties.
  void init (const PortableGroup::Properties & props);

protected:

  /// Destructor.
  ~TAO_LB_LeastLoaded (void);

  /// Retrieve the least loaded location from the given list of
  /// locations.
  CORBA::Boolean get_location (CosLoadBalancing::LoadManager_ptr load_manager,
                               const PortableGroup::Locations & locations,
                               PortableGroup::Location & location);

  /// Return the effective load.
  CORBA::Float effective_load (CORBA::Float previous_load,
                               CORBA::Float new_load);

  /// Push the new load into this Strategy's load processor, and
  /// return the corresponding effective load.
  void push_loads (
      const PortableGroup::Location & the_location,
      const CosLoadBalancing::LoadList & loads,
      CosLoadBalancing::Load & effective_load);

  /// Utility method to extract a CORBA::Float value from the given
  /// property.
  void extract_float_property (const PortableGroup::Property & property,
                               CORBA::Float & value);

private:

  /// This servant's default POA.
  PortableServer::POA_var poa_;

  /// Table that maps location to load list.
  TAO_LB_LoadMap * load_map_;

  /// Lock used to ensure atomic access to state retained by this
  /// class.
  TAO_SYNCH_MUTEX * lock_;

  /// Cached set of properties used when initializing this strategy.
  CosLoadBalancing::Properties properties_;

  /**
   * @name LeastLoaded Property Values
   *
   * Cached LeastLoaded load balancing strategy property values.
   */
  //@{

  /// The critical load threshold.
  /**
   * Load rebalancing/shedding will occur if loads at a given location
   * are greater than this value.  The default value is zero, which
   * indicates that no load rebalancing/shedding will occur.
   */
  CORBA::Float critical_threshold_;

  /// The request rejection threshold.
  /**
   * If the load at a given location is greater than this value, the
   * member at that location will not be considered when choosing a
   * member to the request will be forwarded.  If all locations have
   * loads greater than this value, a @c CORBA::TRANSIENT exception
   * will be thrown unless the reject threshold is zero, which is the
   * default.  If the request rejection threshold is zero, a @c
   * CORBA::TRANSIENT will not be thrown by this load balancing
   * strategy, unless no object group members have been added yet.
   */
  CORBA::Float reject_threshold_;

  ///
  CORBA::Float tolerance_;

  ///
  CORBA::Float dampening_;

  ///
  CORBA::Float per_balance_load_;

  //@}

};

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (__ACE_INLINE__)
#include "orbsvcs/LoadBalancing/LB_LeastLoaded.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif  /* LB_LEAST_LOADED_H */
