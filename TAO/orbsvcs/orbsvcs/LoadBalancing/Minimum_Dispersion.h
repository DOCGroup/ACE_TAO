// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_LoadBalancing
//
// = FILENAME
//    Minimum_Dispersion_Strategy.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef MINIMUM_DISPERSION_H
#define MINIMUM_DISPERSION_H

#include "ace/pre.h"

#include "LoadBalancing_Strategy.h"
#include "orbsvcs/LoadBalancingS.h"
#include "ace/Containers.h"
#include "ace/Synch.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Ossama: my class names suck too, any idea on what would be a
// good name for this strategy? It basically tries to keep all the
// loads "close to the average",  if one server is much higher than
// the load it is deemed too loaded and the advisory is sent.

// Forward declarations
class TAO_LB_ReplicaProxy;

typedef ACE_Unbounded_Set<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySet;
typedef ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySetIterator;

class TAO_LoadBalancing_Export TAO_LB_Minimum_Dispersion_Strategy : public TAO_LB_LoadBalancing_Strategy
{
  // = TITLE
  //    Minimum Dispersion load balancing strategy

  // = DESCRIPTION
  //

public:
  TAO_LB_Minimum_Dispersion_Strategy (void);
  // Constructor.

  ~TAO_LB_Minimum_Dispersion_Strategy (void);
  // Destructor

  // = The TAO_LB_LoadBalancing_Strategy methods
  virtual CORBA::Object_ptr replica (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual int insert (TAO_LB_ReplicaProxy *);
  virtual int remove (TAO_LB_ReplicaProxy *);
  virtual void load_changed (TAO_LB_ReplicaProxy *
                             TAO_ENV_ARG_DECL);

private:
  TAO_LB_ReplicaProxySet proxies_;
  // Set containing the ReplicaProxy servants.

  TAO_SYNCH_MUTEX lock_;
  // Lock used to ensure atomic access to state retained by this
  // class.
};

#include "ace/post.h"

#endif  /* MINIMUM_DISPERSION_H */
