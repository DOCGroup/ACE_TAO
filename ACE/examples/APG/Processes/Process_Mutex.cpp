// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/Process.h"
#include "ace/Process_Mutex.h"

// Listing 1 code/ch10
class GResourceUser
{
public:
  GResourceUser (ACE_Process_Mutex &mutex) : gmutex_(mutex)
  {
    ACE_TRACE ("GResourceUser::GResourceUser");
  }

  void run (void)
  {
    ACE_TRACE ("GResourceUser::run");

    int count = 0;
    while (count++ < 10)
      {
        int result = this->gmutex_.acquire ();
        ACE_TEST_ASSERT (result == 0);

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P| %t) has the mutex\n")));

        // Access Global resource
        ACE_OS::sleep (1);

        result = this->gmutex_.release ();
        ACE_TEST_ASSERT (result == 0);
        ACE_OS::sleep (1);     // Give other process a chance.
      }
  }

private:
  ACE_Process_Mutex &gmutex_;
};
// Listing 1

// Listing 0 code/ch10
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)      // Run as the child.
    {
      // Create or get the global mutex.
      ACE_Process_Mutex mutex ("GlobalMutex");

      GResourceUser acquirer (mutex);
      acquirer.run ();
    }
  else              // Run as the parent.
    {
      ACE_Process_Options options;
      options.command_line (ACE_TEXT ("%s a"), argv[0]);
      ACE_Process processa, processb;

      pid_t pida = processa.spawn (options);
      pid_t pidb = processb.spawn (options);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Spawned processes; pids %d:%d\n"),
                  pida, pidb));

      if (processa.wait() == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("processa wait")), -1);

      if (processb.wait() == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("processb wait")), -1);
    }

  return 0;
}
// Listing 0
