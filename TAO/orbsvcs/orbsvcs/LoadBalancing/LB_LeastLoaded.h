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

#include "ace/pre.h"

#include "LB_LoadMap.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingS.h"

#include "ace/Synch.h"

/**
 * @class TAO_LB_LeastLoaded
 *
 * @brief "Least loaded" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at a location with the smallest load.
 */
class TAO_LoadBalancing_Export TAO_LB_LeastLoaded
  : public virtual POA_CosLoadBalancing::Strategy
{
public:
  typedef TAO_LB_LoadMap LoadMap;

  /// Constructor.
  TAO_LB_LeastLoaded (void);

  /// Destructor
  ~TAO_LB_LeastLoaded (void);

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
    
  virtual void analyze_loads (
      PortableGroup::ObjectGroup_ptr object_group,
      CosLoadBalancing::LoadManager_ptr load_manager
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:

  /// Retrieve the least loaded location from the given list of
  /// locations.
  CORBA::Boolean get_location (const PortableGroup::Locations & locations,
                               PortableGroup::Location & location);


  /// Return the effective load.
  CORBA::Float effective_load (CORBA::Float previous_load,
                               CORBA::Float new_load);

private:

  /// Container that maps location to load list.
  LoadMap load_map_;

  /// Lock used to ensure atomic access to state retained by this
  /// class.
  TAO_SYNCH_MUTEX lock_;

  CORBA::Float critical_threshold_;
  CORBA::Float reject_threshold_;
  CORBA::Float tolerance_;
  CORBA::Float dampening_;
  CORBA::Float per_balance_load_;

};


#if defined (__ACE_INLINE__)
#include "LB_LeastLoaded.inl"
#endif /* defined INLINE */

#include "ace/post.h"

#endif  /* LB_LEAST_LOADED_H */
