// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Sig_Handler.h"

// Listing 1
class MySignalHandler : public ACE_Event_Handler
  {
  public:
    virtual int handle_signal(int signo,
                              siginfo_t * = 0, ucontext_t * = 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Signal %d\n"), signo));
      return 0;
    }
  };

int ACE_TMAIN (int, ACE_TCHAR *[])
{

  MySignalHandler sighandler;
  ACE_Sig_Handler sh;
  sh.register_handler (SIGUSR1, &sighandler);

  ACE_Sig_Set ss;
  ss.sig_add (SIGUSR1);

  ACE_Sig_Guard guard (&ss);
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Entering critical region\n")));
    ACE_OS::sleep (10);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Leaving  critical region\n")));
  }

  // Do other stuff.

  return 0;
}
// Listing 1
