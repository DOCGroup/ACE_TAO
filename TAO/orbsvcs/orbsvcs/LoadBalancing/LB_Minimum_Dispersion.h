// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_Minimum_Dispersion_Strategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef LB_MINIMUM_DISPERSION_H
#define LB_MINIMUM_DISPERSION_H

#include "ace/pre.h"

#include "LB_Balancing_Strategy.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingS.h"
#include "ace/Containers.h"
#include "ace/Synch.h"

// @@ Ossama: my class names suck too, any idea on what would be a
// good name for this strategy? It basically tries to keep all the
// loads "close to the average",  if one server is much higher than
// the load it is deemed too loaded and the advisory is sent.

// Forward declarations
// class TAO_LB_ReplicaProxy;
struct TAO_LB_ObjectGroup_Map_Entry;

// typedef ACE_Unbounded_Set<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySet;
// typedef ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySetIterator;

/**
 * @class TAO_LB_Minimum_Dispersion_Strategy
 *
 * @brief Minimum Dispersion load balancing strategy
 */
class TAO_LoadBalancing_Export TAO_LB_Minimum_Dispersion_Strategy
  : public TAO_LB_Balancing_Strategy
{
public:

  /// Constructor.
  TAO_LB_Minimum_Dispersion_Strategy (void);

  /// Destructor
  ~TAO_LB_Minimum_Dispersion_Strategy (void);

  /**
   * @name The TAO_LB_LoadBalancing_Strategy methods
   */
  //@{

  virtual CORBA::Object_ptr replica (TAO_LB_ObjectGroup_Map_Entry *entry,
                                     CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void analyze_loads (TAO_LB_Location_Map &location_map,
                              CORBA::Environment &ACE_TRY_ENV);

  //@}

private:

//   /// Lock used to ensure atomic access to state retained by this
//   /// class.
//   TAO_SYNCH_MUTEX lock_;

};

#include "ace/post.h"

#endif  /* LB_MINIMUM_DISPERSION_H */
