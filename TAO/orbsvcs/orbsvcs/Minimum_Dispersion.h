// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Minimum_Dispersion_Strategy.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef MINIMUM_DISPERSION_H
#define MINIMUM_DISPERSION_H

#include "LoadBalancing_Strategy.h"
#include "orbsvcs/LoadBalancingS.h"
#include "ace/Containers.h"

// Forward declarations
class ReplicaProxy_Impl;

typedef ACE_Unbounded_Set<ReplicaProxy_Impl *> ReplicaProxySet;
typedef ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl *> ReplicaProxySetIterator;

class TAO_LoadBalancing_Export Minimum_Dispersion_Strategy : public Load_Balancing_Strategy
{
  // = TITLE
  //    Round Robin load balancing strategy

  // = DESCRIPTION
  //    Simple load balancing strategy that causes requests to be
  //    forwarded to next Replica in the set, i.e. in a "round robin"
  //    fashion.

public:
  Minimum_Dispersion_Strategy (void);
  // Constructor.

  ~Minimum_Dispersion_Strategy (void);
  // Destructor

  // = The Load_Balancing_Strategy methods
  virtual CORBA::Object_ptr replica (CORBA_Environment &ACE_TRY_ENV);
  virtual int insert (ReplicaProxy_Impl *);
  virtual int remove (ReplicaProxy_Impl *);
  virtual void load_changed (ReplicaProxy_Impl *,
                             CORBA::Environment &ACE_TRY_ENV);

private:
  ReplicaProxySet proxies_;
  // Set containing the ReplicaProxy servants.
};

#endif  /* MINIMUM_DISPERSION_H */
