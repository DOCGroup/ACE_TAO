
//=============================================================================
/**
 *  @file   Pool_Per_Endpoint.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_POOL_PER_ENDPOINT_H
#define TAO_POOL_PER_ENDPOINT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

/**
 * @class TAO_Pool_Per_Endpoint
 *
 * @brief The Pool_Per_Endpoint concurrency strategy.
 *
 * This strategy creates a separate thread pool for each endpoint
 * in the system.  It is a helper class to manage and create the
 * required threads.
 * It should be used with the Reactor_Per_Endpoint registry
 * configured.
 */
class TAO_Export TAO_Pool_Per_Endpoint : public ACE_Task_Base
{
public:
  /// Constructor
  TAO_Pool_Per_Endpoint (CORBA::ORB_ptr orb,
                         int policy = ACE_SCHED_OTHER,
                         int poolsize = 1,
                         int flags =
                             THR_BOUND|THR_NEW_LWP|THR_JOINABLE);

  /// The destructor
  virtual ~TAO_Pool_Per_Endpoint (void);

  /// Run all the threads and wait until they finish
  void run (CORBA::Environment &ACE_TRY_ENV =
                TAO_default_environment ());

  // = The Task_Base entry point
  int svc (void);

private:
  /// The ORB.
  CORBA::ORB_var orb_;

  /// The scheduling policy to activate the threads.
  int policy_;

  /// The number of threads on each pool.
  int poolsize_;

  /// The flags used to create the threads
  int flags_;

  /// The thread manager.
  ACE_Thread_Manager thread_manager_;
};

#if defined (__ACE_INLINE__)
# include "tao/Pool_Per_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_POOL_PER_ENDPOINT_H */
