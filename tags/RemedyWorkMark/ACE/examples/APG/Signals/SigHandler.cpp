// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Sig_Handler.h"

// Listing 1 code/ch11
class MySignalHandler : public ACE_Event_Handler
{
public:
  MySignalHandler (int signum) : signum_(signum)
  { }

  virtual ~MySignalHandler()
  { }

  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0)
  {
    ACE_TRACE (ACE_TEXT ("MySignalHandler::handle_signal"));

    // Make sure the right handler was called back.
    ACE_ASSERT (signum == this->signum_);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%S occured\n"), signum));
    return 0;
  }

private:
  int signum_;
};
// Listing 1
// Listing 2 code/ch11
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  MySignalHandler h1 (SIGUSR1), h2 (SIGUSR2);
  ACE_Sig_Handler handler;
  handler.register_handler (SIGUSR1, &h1);
  handler.register_handler (SIGUSR2, &h2);

  ACE_OS::kill (ACE_OS::getpid (), SIGUSR1);
  ACE_OS::kill (ACE_OS::getpid (), SIGUSR2);

  int time = 10;
  while ((time = ACE_OS::sleep (time)) == -1)
    {
      if (errno == EINTR)
        continue;
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("sleep")), -1);
        }
    }
  return 0;
}
// Listing 2
