// $Id$

#if !defined (GLOBALS_H)
#define GLOBALS_H

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Globals.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/Synch_T.h"

#if !defined (ACE_HAS_THREADS)
class NOOP_ACE_Barrier
{
public:
  NOOP_ACE_Barrier (int ) {}
  void wait (void) {}
};
#define ACE_Barrier NOOP_ACE_Barrier
#endif /* ACE_HAS_THREADS */

class Globals
{
  // = TITLE
  //   // @@ Naga, can you please fill in here?
  // = DESCRIPTION
  //   // @@ Naga, can you please fill in here?
public:
  Globals (void);

  // @@ Naga, can you please make sure these fields/methods are
  // commented briefly?
  int parse_args (int argc,char **argv);
  char hostname[BUFSIZ];
  char *ior_file;
  int base_port;
  u_int num_of_objs;
  u_int use_name_service;
  u_int thread_per_rate;
  u_int use_multiple_priority;

  int ready_;
  // ready flag used by the high priority thread to wake up the low
  // priority threads after it's parsed the arguments.

  ACE_SYNCH_MUTEX ready_mtx_;
  // mutex for the condition variable.

  ACE_Condition<ACE_SYNCH_MUTEX> ready_cnd_;
  // condition variable for the low priority threads to wait 
  //until the high priority thread is done with the arguments parsing.
  
  ACE_Barrier *barrier_;
  // Barrier for the multiple servants to synchronize after
  // binding to the orb.
};

#endif /* GLOBALS_H */




