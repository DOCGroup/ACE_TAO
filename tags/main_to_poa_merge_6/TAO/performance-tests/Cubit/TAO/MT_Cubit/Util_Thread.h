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
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "Task_Client.h"

#define CUBIT_ARBIT_NUMBER 2064885

class Util_Thread : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //     The thread that computes utilization
{
public:
  Util_Thread (Task_State *, ACE_Thread_Manager *thr_mgr);
  // constructor

  virtual int svc (void);
  // the thread function

  double get_number_of_computations (void);
  // gets the number of computations the thread was able
  // to perform

  int done_;
  // Indicates whether its done.

private:

  int run_computations ();
  // run the computations

  double number_of_computations_;
  Task_State *ts_;
};
