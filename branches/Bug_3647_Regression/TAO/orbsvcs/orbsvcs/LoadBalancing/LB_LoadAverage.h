// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_LoadAverage.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef LB_LOAD_AVERAGE_H
#define LB_LOAD_AVERAGE_H

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
   * @name Default LoadAverage strategy parameters.
   *
   * Defaults defined by the Load Balancing specification.
   */
  //@{
  const CORBA::Float LA_DEFAULT_TOLERANCE = 1;
  const CORBA::Float LA_DEFAULT_DAMPENING = 0;
  const CORBA::Float LA_DEFAULT_PER_BALANCE_LOAD = 0;

  const CORBA::Float LA_DEFAULT_DIFF_AVERAGE_CUTOFF = 0.8f;
  //@}

}

/**
 * @class TAO_LB_LoadAverage
 *
 * @brief "Least loaded" load balancing strategy
 *
 * This load balancing strategy is designed to select an object group
 * member residing at a location with the smallest load.
 */
class TAO_LB_LoadAverage
  : public virtual POA_CosLoadBalancing::Strategy
{
public:

  /// Constructor.
  TAO_LB_LoadAverage (PortableServer::POA_ptr poa);

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

  /// Initialize the LoadAverage instance with the given properties.
  void init (const PortableGroup::Properties & props);

protected:

  /// Destructor.
  ~TAO_LB_LoadAverage (void);

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
   * @name LoadAverage Property Values
   *
   * Cached LoadAverage load balancing strategy property values.
   */
  //@{

  ///
  CORBA::Float tolerance_;

  ///
  CORBA::Float dampening_;

  ///
  CORBA::Float per_balance_load_;

  //@}

  CosLoadBalancing::LoadList current_loads_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/LoadBalancing/LB_LoadAverage.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif  /* LB_LOAD_AVERAGE_H */
