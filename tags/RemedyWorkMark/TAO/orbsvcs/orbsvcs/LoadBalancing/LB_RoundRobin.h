// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_RoundRobin.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_ROUND_ROBIN_H
#define LB_ROUND_ROBIN_H

#include /**/ "ace/pre.h"

#include "ace/Null_Mutex.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancing/LB_Location_Index_Map.h"

#include "orbsvcs/CosLoadBalancingS.h"
#include "ace/Vector_T.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_RoundRobin_Strategy
 *
 * @brief "Round Robin" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at the next location.
 */
class TAO_LB_RoundRobin
  : public virtual POA_CosLoadBalancing::Strategy
{
public:

  /// Constructor.
  TAO_LB_RoundRobin (PortableServer::POA_ptr poa);

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

protected:

  /// Destructor
  ~TAO_LB_RoundRobin (void);

private:

  /// Keep a copy of location list for verify if the list is changed
  /// in next next_member() call.
  void copy_locations (PortableGroup::Locations_var& locations);

  /// This servant's default POA.
  PortableServer::POA_var poa_;

  /// Lock used to ensure atomic access to state retained by this
  /// class.
  TAO_SYNCH_MUTEX lock_;

  /// Table that maps PortableGroup::ObjectGroupId to location
  /// sequence index specific to a given object group.
  /**
   * The location sequence corresponds to the sequence containing the
   * locations of the members of a given object group.  The value
   * stored in this map corresponds to the index of the next element
   * in that sequence.  For example, if the index stored in the map is
   * 2, location[2] will be used when retrieving the object reference
   * to be returned from the Strategy::next_member() method.
   */
  TAO_LB_Location_Index_Map location_index_map_;

  /// Locations list retrieved in last next_member() call.
  ACE_Vector <PortableGroup::Location> last_locations_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* LB_ROUND_ROBIN_H */
