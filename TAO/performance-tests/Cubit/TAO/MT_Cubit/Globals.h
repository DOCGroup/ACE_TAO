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
  //     Globals class to be used as a singleton.
  // = DESCRIPTION
  //     This is used both by the server and client side.
public:
  Globals (void);

  // @@ Naga, can you please make sure these fields/methods are
  // commented briefly?
  int parse_args (int argc,char **argv);
  // parse the arguments.

  char hostname[BUFSIZ];
  // hostname to be used for ORB_init.

  char *ior_file;
  // file name to read/write the iors of the servants.

  int base_port;
  // base_port at which the servants will be listening.The high
  // priority servant will listen at the base_port and the
  // low_priority servants will listen at base_port+i where i is the
  // number of that low priority servant.

  u_int num_of_objs;
  // number of objects per servant thread.

  u_int use_name_service;
  // flag for usage of naming service.

  u_int thread_per_rate;
  //  thread_per_rate test flag.

  u_int use_multiple_priority;
  // flag to use multiple priorities for the low  priority servants
  // instead of one priority.

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




