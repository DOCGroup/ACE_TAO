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

#include "LB_LoadMap.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingC.h"

#include "ace/Synch.h"

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

protected:

  /// Retrieve the least loaded location from the given list of
  /// locations.
//   CORBA::Boolean get_location (const PortableGroup::Locations & locations,
//                                PortableGroup::Location & location);

private:

  /// Lock used to ensure atomic access to state retained by this
  /// class.
//   TAO_SYNCH_MUTEX lock_;

};

#include "ace/post.h"

#endif  /* LB_ROUND_ROBIN_H */
