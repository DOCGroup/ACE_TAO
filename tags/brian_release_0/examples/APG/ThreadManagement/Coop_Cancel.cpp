// $Id$

#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch13
class CanceledTask : public ACE_Task<ACE_MT_SYNCH>
{
public:

  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up \n")));

    // Cache our ACE_Thread_Manager pointer.
    ACE_Thread_Manager *mgr = this->thr_mgr ();
    while (1)
      {
        if (mgr->testcancel (mgr->thr_self ()))
          return 0;

        ACE_Message_Block *mb;
        ACE_Time_Value tv (0, 1000);
        tv += ACE_OS::time (0);
        int result = this->getq (mb, &tv);
        if (result == -1 && errno == EWOULDBLOCK)
          continue;
        else
          {
            // Do real work.
          }
      }

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

  ACE_Thread_Manager::instance ()->cancel_task (&task);
  task.wait ();
  return 0;
}
// Listing 2
