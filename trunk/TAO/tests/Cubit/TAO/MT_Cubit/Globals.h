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
#include "ace/Sched_Params.h"

#if !defined (ACE_HAS_THREADS)
class NOOP_ACE_Barrier
{
public:
  NOOP_ACE_Barrier (int ) {}
  void wait (void) {}
};
#define ACE_Barrier NOOP_ACE_Barrier
#endif /* ACE_HAS_THREADS */

#if defined (VXWORKS) && defined (VME_DRIVER)
#define VX_VME_INIT \
STATUS status = vmeDrv ();\
if (status != OK)\
     ACE_DEBUG ((LM_DEBUG,\
                "ERROR on call to vmeDrv()\n"));\
  status = vmeDevCreate ("/vme");\
  if (status != OK)\
    ACE_DEBUG ((LM_DEBUG,\
                "ERROR on call to vmeDevCreate()\n"));
#else
#define VX_VME_INIT
#endif /* VXWORKS && VME_DRIVER */

#if defined (ACE_LACKS_FLOATING_POINT)
#define TIME_IN_MICROSEC(X) X
#else /* !ACE_LACKS_FLOATING_POINT */
#define TIME_IN_MICROSEC(X) \
(X * ACE_ONE_SECOND_IN_USECS)
#endif /* !ACE_LACKS_FLOATING_POINT */

class Globals
{
  // = TITLE
  //     Globals class to be used as a singleton.
  // = DESCRIPTION
  //     This is used both by the server and client side.
public:
  Globals (void);
  // default constructor.

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

class MT_Priority
{
public:
  MT_Priority (void);
  // constructor.

  virtual ACE_Sched_Priority get_high_priority (void);
  // sets the priority of the high priority thread.

  virtual ACE_Sched_Priority get_low_priority (u_int num_low_priority,
                                               ACE_Sched_Priority prev_priority,
                                               u_int use_multiple_priority);
  // sets the priority to be used for the low priority thread.
  u_int number_of_priorities (void);
  // accessor for num_priorities_.

  u_int grain (void);
  // accessor for grain_.

protected:
  u_int num_priorities_;
  // number of priorities used.

  u_int grain_;
  // Granularity of the assignment of the priorities.  Some OSs
  // have fewer levels of priorities than we have threads in our
  // test, so with this mechanism we assign priorities to groups
  // of threads when there are more threads than priorities.
};


#endif /* GLOBALS_H */




