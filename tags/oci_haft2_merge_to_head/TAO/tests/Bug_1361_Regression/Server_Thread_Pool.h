// $Id$

// This test program illustrates how the <ACE_Task> synchronization
// mechanisms work in conjunction with the <ACE_Thread_Manager>.  If
// the <manual> flag is set input comes from stdin until the user
// enters a return -- otherwise, the input is generated automatically.
// All worker threads shutdown when they receive a message block of
// length 0.
//
// This code is original based on a test program written by Karlheinz
// Dorn <Karlheinz.Dorn@med.siemens.de>.  It was modified to utilize
// more ACE features by Doug Schmidt <schmidt@cs.wustl.edu>.

#include "ace/OS.h"
#include "ace/Synch_T.h"
#include "ace/Task_T.h"
#include "ace/Service_Config.h"

#include "TestC.h"

class Thread_Pool : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Defines a thread pool abstraction based on the <ACE_Task>.
public:
  Thread_Pool (ACE_Thread_Manager *thr_mgr,
               int n_threads);
  // Constructor activates <n_threads> in the thread pool.

  virtual ~Thread_Pool (void);
  // Destructor...
  int shutdown (void);

  virtual int svc (void);

  virtual int put (Test::Echo_ptr echoptr);

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
  // This allows the producer to pass messages to the <Thread_Pool>.

private:
  virtual int close (u_long);
  int nt_;                      // number of threads
  // Close hook.
};
