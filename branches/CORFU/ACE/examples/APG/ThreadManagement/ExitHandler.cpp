// $Id$

// Listing 1 code/ch13
#include "ace/Task.h"
#include "ace/Log_Msg.h"

class ExitHandler : public ACE_At_Thread_Exit
{
public:
  virtual void apply (void)
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) is exiting\n")));

    // Shut down all devices.
  }
};
// Listing 1
// Listing 2 code/ch13
class HA_CommandHandler : public ACE_Task_Base
{
public:
  HA_CommandHandler(ExitHandler& eh) : eh_(eh)
  { }

  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up\n")));

    this->thr_mgr ()->at_exit (eh_);

    // Do something.

    // Forcefully exit.
    ACE_Thread::exit ();

    // NOT REACHED
    return 0;
  }

private:
  ExitHandler& eh_;
};
// Listing 2
// Listing 3 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ExitHandler eh;

  HA_CommandHandler handler (eh);
  handler.activate ();

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
// Listing 3
#if 0
// Listing 4 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ExitHandler eh;
  ACE_Thread_Manager tm;

  HA_CommandHandler handler (eh);
  handler.thr_mgr (&tm);
  handler.activate ();

  tm.wait();
  return 0;
}
// Listing 4
#endif
