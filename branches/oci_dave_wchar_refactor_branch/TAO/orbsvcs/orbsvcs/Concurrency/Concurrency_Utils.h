// -*- C++ -*-

//=============================================================================
/**
 *  @file    Concurrency_Utils.h
 *
 *  $Id$
 *
 *    This class implements a Concurrency Server wrapper class which
 *    holds a number of lock sets.  The server must run in the
 *    thread per request concurrency model in order to let the
 *    clients block on the semaphores.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#ifndef _CONCURRENCY_SERVER_H
#define _CONCURRENCY_SERVER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/Concurrency/CC_LockSetFactory.h"
#include "orbsvcs/Concurrency/concurrency_serv_export.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Concurrency_Server
 *
 * @brief Defines a wrapper class for the implementation of the
 * concurrency server.
 *
 * This class takes an orb and Poa reference and activates the
 * concurrency service lock set factory object under them.
 */
class TAO_Concurrency_Serv_Export TAO_Concurrency_Server
{
public:
  // = Initialization and termination methods.
  ///Default constructor.
  TAO_Concurrency_Server (void);

  /// Takes the POA under which to register the Concurrency Service
  /// implementation object.
  TAO_Concurrency_Server (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);

  /// Destructor.
  ~TAO_Concurrency_Server (void);

  /// Initialize the concurrency server under the given ORB and POA.
  CORBA::Object_ptr init (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);

  /// Cleanup resources.
  int fini (void);

  /// Get the lock set factory.
  CC_LockSetFactory *GetLockSetFactory (void);

private:
  /// This is the lock set factory activated under the POA.
  CC_LockSetFactory lock_set_factory_;

  /// The POA which the lock set factory servant was registered.
  PortableServer::POA_var poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* _CONCURRENCY_SERVER_H */
