// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_Round_Robin_Strategy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_ROUND_ROBIN_STRATEGY_H
#define LB_ROUND_ROBIN_STRATEGY_H

#include "ace/pre.h"

#include "LB_Balancing_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingS.h"
#include "ace/Synch.h"

// Forward declarations
class TAO_LB_ReplicaProxy;
class TAO_LB_ObjectGroup_Map_Entry;

// @@ Ossama: is this the kind of data structure that you want to use
// in this case? I mean the iterator is invalidated on each insertion
// and extraction, the class is not thread safe and it is hard to find
// the element with the least load...  Then again the number of
// servers should be small (say around 10), and additions/removals
// rare, so the data structure is probably not a real problem.

typedef ACE_Unbounded_Set<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySet;
typedef ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySetIterator;

/**
 * @class TAO_LB_Round_Robin_Strategy
 *
 * @brief Round Robin load balancing strategy
 *
 * Simple load balancing strategy that causes requests to be forwarded
 * to next Replica in the set, i.e. in a "round robin" fashion.
 */
class TAO_LoadBalancing_Export TAO_LB_Round_Robin_Strategy
  : public TAO_LB_Balancing_Strategy
{
public:

  /// Constructor.
  TAO_LB_Round_Robin_Strategy (void);

  /// Destructor
  ~TAO_LB_Round_Robin_Strategy (void);

  /**
   * @name The LB_Balancing_Strategy methods
   */
  //@{

  virtual CORBA::Object_ptr replica (TAO_LB_ObjectGroup_Map_Entry *entry,
                                     CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

};

#include "ace/post.h"

#endif  /* LB_ROUND_ROBIN_STRATEGY_H */
