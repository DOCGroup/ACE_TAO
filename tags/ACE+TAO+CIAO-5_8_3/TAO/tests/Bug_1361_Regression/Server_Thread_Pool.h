// $Id$

#include "ace/Task_T.h"
#include "ace/Service_Config.h"

#include "TestC.h"

class Thread_Pool : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Defines a thread pool abstraction based on the <ACE_Task>.
public:
  Thread_Pool (CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr,
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
  // Close hook.
  CORBA::ORB_var orb_;
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, int> nt_;     // number of threads
};
