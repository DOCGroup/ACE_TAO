/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    client.h
//
// = AUTHOR
//    Andy Gokhale, Brian Mendel, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "cubitC.h"
#include "Task_Client.h"
#include "Util_Thread.h"

#if defined (CHORUS)
#include "pccTimer.h"
#endif /* CHORUS */

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* defined (VME_DRIVER) */

// class Client
// {
// public:
//   Client (void);
//   //constructor.
//   int do_priority_inversion_test (ACE_Thread_Manager *thread_manager,
//                                   Task_State *ts);
//   int do_thread_per_rate_test (ACE_Thread_Manager *thread_manager,
//                                Task_State *ts);
//   int start_servant (Task_State *ts, ACE_Thread_Manager &thread_manager);

//   void output_latency (Task_State *ts);

// #if defined (VXWORKS)
//   void output_taskinfo (void);
// #endif /* VXWORKS */
//   int initialize (void);
// };


