// -*- C++ -*-

//=============================================================================
/**
 *  @file LoadBalancing_Strategy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_BALANCING_STRATEGY_H
#define TAO_LB_BALANCING_STRATEGY_H

#include "ace/pre.h"

#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingS.h"
#include "LB_Location_Map.h"


// class TAO_LB_ReplicaProxy;
struct TAO_LB_ObjectGroup_Map_Entry;


/**
 * @class TAO_LB_Balancing_Strategy
 *
 * @brief Load balancing strategy abstract base class.
 *
 * This abstract base class defines the interface load balancing
 * strategies should implement.
 */
class TAO_LoadBalancing_Export TAO_LB_Balancing_Strategy
{
public:

  /// Destructor
  virtual ~TAO_LB_Balancing_Strategy (void);

  // @@ The name of the method should be more meaningful, what about:
  //    select_replica(), select_unloaded_replica() or something like
  //    that?
  /// Return the object reference to the Replica to which requests should
  /// be redirected.
  virtual CORBA::Object_ptr replica (TAO_LB_ObjectGroup_Map_Entry *entry,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Analyze the loads at each location and force client request
  /// redirection, if necessary, or disable the redirection at a given
  /// location.
  /**
   * @@ Ossama: see my comments on LoadBalancing_i about changing the
   * signature of this method.
   */
  virtual void analyze_loads (TAO_LB_Location_Map &location_map,
                              CORBA::Environment &ACE_TRY_ENV);

};


#include "ace/post.h"

#endif  /* TAO_LB_BALANCING_STRATEGY_H */
