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
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (UTIL_THREAD_H)
#define UTIL_THREAD_H

#include "Task_Client.h"

// @@ Please comment me.
const int CUBIT_ARBIT_NUMBER = 2064885;

class Util_Thread : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //     The thread that computes utilization.
public:
  Util_Thread (Task_State *,
               ACE_Thread_Manager *thr_mgr);
  // Constructor.

  virtual int svc (void);
  // The thread entry point function.

  double get_number_of_computations (void);
  // Gets the number of computations the thread was able to perform.

  int done_;
  // Indicates whether we're done.

private:
  int run_computations (void);
  // Run the computations.

  // @@ Sumedh, please add comments here.
  double number_of_computations_;
  Task_State *ts_;
};

#endif /* !defined (UTIL_THREAD_H) */
