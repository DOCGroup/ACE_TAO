/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Util_Thread.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, Sergio Flores-Gaitan and Nagarajan
//    Surendran.
//
// ============================================================================

#ifndef UTIL_THREAD_H
#define UTIL_THREAD_H

#include "Task_Client.h"

class Util_Thread : public ACE_Task<ACE_NULL_SYNCH>
{
  // = TITLE
  //     The thread that computes utilization.
public:
  Util_Thread (Task_State *,
               ACE_Thread_Manager *thr_mgr);
  // Constructor.

  virtual int svc (void);
  // The thread entry point function.

  u_long get_number_of_computations (void);
  // Gets the number of computations the thread was able to perform.

  void computation (void);
  // this is the actual "computation" being performed.

  int done_;
  // Indicates whether we're done.

private:
  int run_computations (void);
  // Run the computations.

  u_long number_of_computations_;
  // This stores the number of computations performed
  // by the Util_Thread so far

  Task_State *ts_;
  // Pointer to the Task state. Stores the state
  // common to the multiple client threads
};

#endif /* !defined (UTIL_THREAD_H) */
