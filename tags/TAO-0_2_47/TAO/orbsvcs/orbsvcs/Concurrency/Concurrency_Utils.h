/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    Concurrency_Utils.h
//
// = DESCRIPTION
//      This class implements a Concurrency Server wrapper class which
//      holds a number of lock sets.  The server must run in the
//      thread per request concurrency model in order to let the
//      clients block on the semaphores.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#ifndef _CONCURRENCY_SERVER_H
#define _CONCURRENCY_SERVER_H

#include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "CC_LockSetFactory.h"

class TAO_ORBSVCS_Export TAO_Concurrency_Server
{
  // = TITLE
  //    Defines a wrapper class for the implementation of the
  //    concurrency server.
  //
  // = DESCRIPTION
  //    This class takes an orb and Poa reference and activates the
  //    concurrency service lock set factory object under them.
public:
  // = Initialization and termination methods.
  TAO_Concurrency_Server (void);
  //Default constructor.

  TAO_Concurrency_Server (CORBA::ORB_var &orb,
                          PortableServer::POA_var &poa);
  // Takes the POA under which to register the Concurrency Service
  // implementation object.

  ~TAO_Concurrency_Server (void);
  // Destructor.

  int init (CORBA::ORB_var &orb,
            PortableServer::POA_var &poa);
  // Initialize the concurrency server under the given ORB and POA.

  CC_LockSetFactory *GetLockSetFactory(void);
  // Get the lock set factory.

private:
  CC_LockSetFactory lock_set_factory_;
  // This is the lock set factory activated under the POA.
};

#endif /* _CONCURRENCY_SERVER_H */

