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
#include "ace/pre.h"

#include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "CC_LockSetFactory.h"
#include "concurrency_export.h"

class TAO_Concurrency_Export TAO_Concurrency_Server
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

  TAO_Concurrency_Server (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);
  // Takes the POA under which to register the Concurrency Service
  // implementation object.

  ~TAO_Concurrency_Server (void);
  // Destructor.

  CORBA::Object_ptr init (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);
  // Initialize the concurrency server under the given ORB and POA.

  /// Cleanup resources.
  int fini (void);

  CC_LockSetFactory *GetLockSetFactory (void);
  // Get the lock set factory.

private:
  CC_LockSetFactory lock_set_factory_;
  // This is the lock set factory activated under the POA.

  /// The POA which the lock set factory servant was registered.
  PortableServer::POA_var poa_;
};

#include "ace/post.h"
#endif /* _CONCURRENCY_SERVER_H */
