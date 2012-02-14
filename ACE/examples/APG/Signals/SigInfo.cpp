// $Id$

#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Signal.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"

#if !defined (ACE_LACKS_UNIX_SIGNALS)

// Listing 1 code/ch11
class MySignalHandler : public ACE_Event_Handler
{
public:
  MySignalHandler () : ACE_Event_Handler()
  { }

  // Listing A code/ch11
  int handle_signal (int signum,
                     siginfo_t * siginfo = 0,
                     ucontext_t * = 0)
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("Received signal [%S]\n"),
                signum));
    if (siginfo == 0)
      {
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("No siginfo_t available for ")
                    ACE_TEXT ("signal [%S]\n"),
                    signum));
        return 0;
      }
    // Listing A
#if defined (ACE_LINUX)
    // Listing B code/ch11
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("errno for this signal is %d [%s]\n"),
                siginfo->si_errno,
                ACE_OS::strerror (siginfo->si_errno)));
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("signal was sent by process %d")
                ACE_TEXT (" / user %d\n"),
                siginfo->si_pid,
                siginfo->si_uid));

    switch (siginfo->si_code)
      {
      case SI_TIMER:
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("Timer expiration\n")));
        break;

      case SI_USER:
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Sent by kill, sigsend or raise\n")));
        break;

      case SI_KERNEL:
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Sent by kernel\n")));
        break;
        // ...
      };

    // Listing B

    // Listing C code/ch11
    switch (signum)
      {
      case SIGFPE:
        switch (siginfo->si_code)
          {
          case FPE_INTDIV:
          case FPE_FLTDIV:
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("Divide by zero at %@\n"),
                        siginfo->si_addr));
            break;

          case FPE_INTOVF:
          case FPE_FLTOVF:
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("Numeric overflow at %@\n"),
                        siginfo->si_addr));
            break;

          // ...
          }
        break;

        // Listing C
        // Listing D code/ch11
      case SIGSEGV:
        switch (siginfo->si_code)
          {
            // ...
          };
        break;
        // Listing D

        // Listing E code/ch11
      case SIGCHLD:
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("A child process has exited\n")));
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("The child consumed %l/%l time\n"),
                    siginfo->si_utime,
                    siginfo->si_stime));
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("and exited with value %d\n"),
                    siginfo->si_status));
        break;
        // ...
      }
    // Listing E
#endif /* __linux__ */
    return 0;
  }
};
// Listing 1

#endif /* ACE_LACKS_UNIX_SIGNALS */

#if !defined (ACE_LACKS_UNIX_SIGNALS)

int ACE_TMAIN (int, ACE_TCHAR *[])
{
#if defined (ACE_LACKS_FORK)
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              "This example requires fork()\n"));
  //FUZZ: enable check_for_lack_ACE_OS
#else
  // Create a child process so that we can test our
  // ability to handle SIGCHLD

  // Listing 2 code/ch11
  ACE_Sig_Set signalSet;
  signalSet.fill_set ();

  MySignalHandler h1;
  ACE_Reactor::instance ()->register_handler (signalSet, &h1);
  pid_t childPid = ACE_OS::fork ();
  if (childPid == 0)      // This is the parent process.
    {
      // Exclude B
      ACE_OS::sleep (10);
      return 100;
      // Exclude B
    }
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  // Listing 2

#endif /* ACE_LACKS_FORK */

  return 0;
}

#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example does not work on this platform.\n"));
  return 1;
}

#endif /* !ACE_LACKS_UNIX_SIGNALS */
