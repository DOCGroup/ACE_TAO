/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Util_Thread.h
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


#ifndef UTIL_THREAD_H
#define UTIL_THREAD_H

#include "Task_Client.h"

/**
 * @class Util_Thread
 *
 * @brief The thread that computes utilization.
 */
class Util_Thread : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  /// Constructor.
  Util_Thread (Task_State *,
               ACE_Thread_Manager *thr_mgr);

  /// The thread entry point function.
  virtual int svc (void);

  /// Gets the number of computations the thread was able to perform.
  u_long get_number_of_computations (void);

  /// this is the actual "computation" being performed.
  void computation (void);

  /// Indicates whether we're done.
  int done_;

private:
  /// Run the computations.
  int run_computations (void);

  /// This stores the number of computations performed
  /// by the Util_Thread so far
  u_long number_of_computations_;

  /// Pointer to the Task state. Stores the state
  /// common to the multiple client threads
  Task_State *ts_;
};

#endif /* !defined (UTIL_THREAD_H) */
