// -*- C++ -*-

//=============================================================================
/**
 * @file   Signal_Handler.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_SIGNAL_HANDLER_H
#define TAO_LB_SIGNAL_HANDLER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServerC.h"


/**
 * @class TAO_LB_Signal_Handler
 *
 * @brief Active object designed to handle signals synchronously.
 *
 * This class handles signals synchronously in the multi-threaded case,
 * and asynchronously through a Reactor in the single-threaded case.
 * @par
 * The goal of this signal handler is to ensure proper cleanup of
 * resources created by servants upon interruption by signals.  For
 * example, suppose a user sends a
 * SIGINT (e.g. types CTRL-C) signal to an application.  The servants
 * in that application should be shutdown gracefully so that all
 * resources handled by those servants are cleaned up.  This is
 * particularly important for resources that are not cleaned up
 * automatically.
 */
class TAO_LB_Signal_Handler : public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_LB_Signal_Handler (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa);

  /// Run by a daemon thread to handle signals synchronously.
  virtual int svc (void);

  /// Active object activation method.
  /**
   * This override blocks all signals in the calling thread before
   * spawning the synchronous signal handler.
   */
  virtual int activate (long flags = THR_NEW_LWP | THR_JOINABLE,
                        int n_threads = 1,
                        int force_active = 0,
                        long priority = ACE_DEFAULT_THREAD_PRIORITY,
                        int grp_id = -1,
                        ACE_Task_Base *task = 0,
                        ACE_hthread_t thread_handles[] = 0,
                        void *stack[] = 0,
                        size_t stack_size[] = 0,
                        ACE_thread_t thread_ids[] = 0);


  /// Called when object is signaled by OS (either via UNIX signals or
  /// when a Win32 object becomes signaled).
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

protected:

  /// Template method that initiates the cleanup process.
  virtual int perform_cleanup (int signum);

private:

#ifdef ACE_HAS_THREADS
  /// Block all signals before spawning the threads.  Then, unblock
  /// these signals when this handler is destroyed.
  ACE_Sig_Guard signal_guard_;
#endif  /* ACE_HAS_THREADS */

  /// The set of signals which this handler will handle.
  ACE_Sig_Set sigset_;

  /// Pseudo-reference to the ORB in which the LoadManager is running.
  CORBA::ORB_var orb_;

  /// Reference to the POA within which the LoadManager servant is
  /// registered.
  PortableServer::POA_var poa_;

};


#endif  /* TAO_LB_SIGNAL_HANDLER_H */
