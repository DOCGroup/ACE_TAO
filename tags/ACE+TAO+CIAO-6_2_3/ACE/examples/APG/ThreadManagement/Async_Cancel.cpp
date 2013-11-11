// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_CANCEL)
// Only works on Pthreads...

// Listing 1 code/ch13
class CanceledTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Starting thread\n")));

    if (this->set_cancel_mode () < 0)
      return -1;

    while (1)
      {
        // Put this thread in a compute loop.. no
        // cancellation points are available.
      }
  }

  int set_cancel_mode (void)
  {
    cancel_state new_state;

    // Set the cancel state to asynchronous and enabled.
    new_state.cancelstate = PTHREAD_CANCEL_ENABLE;
    new_state.canceltype  = PTHREAD_CANCEL_ASYNCHRONOUS;
    if (ACE_Thread::setcancelstate (new_state, 0) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("cancelstate")), -1);
    return 0;
  }
};
// Listing 1
// Listing 2 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  CanceledTask task;
  task.activate ();
  ACE_OS::sleep (1);
  ACE_Thread_Manager::instance ()->cancel_task (&task, 1);
  task.wait ();

  return 0;
}
// Listing 2

#else  /* ACE_HAS_PTHREADS */
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts ("This example works on Pthreads platforms.\n");
  return 0;
}
#endif /* ACE_HAS_PTHREADS */

