// $Id$


// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Pool_Per_Endpoint.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_POOL_PER_ENDPOINT_H
#define TAO_POOL_PER_ENDPOINT_H

#include "tao/corbafwd.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

class TAO_Export TAO_Pool_Per_Endpoint : public ACE_Task_Base
{
  // = TITLE
  //   The Pool_Per_Endpoint concurrency strategy.
  //
  // = DESCRIPTION
  //   This strategy creates a separate thread pool for each endpoint
  //   in the system.  It is a helper class to manage and create the
  //   required threads.
  //   It should be used with the Reactor_Per_Endpoint registry
  //   configured.
  //
public:
  TAO_Pool_Per_Endpoint (CORBA::ORB_ptr orb,
                         int policy = ACE_SCHED_OTHER,
                         int poolsize = 1,
                         int flags =
                             THR_BOUND|THR_NEW_LWP|THR_JOINABLE);
  // Constructor

  virtual ~TAO_Pool_Per_Endpoint (void);
  // The destructor

  void run (CORBA::Environment &ACE_TRY_ENV =
                TAO_default_environment ());
  // Run all the threads and wait until they finish

  // = The Task_Base entry point
  int svc (void);

private:
  CORBA::ORB_var orb_;
  // The ORB.

  int policy_;
  // The scheduling policy to activate the threads.

  int poolsize_;
  // The number of threads on each pool.

  int flags_;
  // The flags used to create the threads

  ACE_Thread_Manager thread_manager_;
  // The thread manager.
};

#if defined (__ACE_INLINE__)
# include "tao/Pool_Per_Endpoint.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_POOL_PER_ENDPOINT_H */
