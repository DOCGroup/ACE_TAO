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

#include "ace/pre.h"

#include "ace/Synch.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancing/LB_Location_Index_Map.h"

#include "orbsvcs/CosLoadBalancingC.h"

/**
 * @class TAO_LB_RoundRobin_Strategy
 *
 * @brief "Round Robin" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at the next location.
 */
class TAO_LB_RoundRobin
  : public virtual CosLoadBalancing::Strategy,
    public virtual CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_LB_RoundRobin (void);

  /// Destructor
  ~TAO_LB_RoundRobin (void);

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

  virtual CORBA::Object_ptr next_member (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));
  //@}

protected:

  /// Retrieve the least loaded location from the given list of
  /// locations.
//   CORBA::Boolean get_location (const PortableGroup::Locations & locations,
//                                PortableGroup::Location & location);

private:

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

};

#include "ace/post.h"

#endif  /* LB_ROUND_ROBIN_H */
