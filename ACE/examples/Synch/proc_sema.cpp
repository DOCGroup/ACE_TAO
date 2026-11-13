#include "ace/OS_main.h"
#include "ace/Process_Semaphore.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Synch_Traits.h"

void producer (ACE_SYNCH_PROCESS_SEMAPHORE &sema,
               int const iter)
{
  for (int i = iter; i > 0; --i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Try releasing the semaphore (%d): ",
                  i));

      int const result = sema.release ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  (result != 0 ? "fail\n" : "succeed\n")));
    }
}

void consumer (ACE_SYNCH_PROCESS_SEMAPHORE &sema,
               int const iter)
{
  for (int i = iter; i > 0; --i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Try acquiring the semaphore (%d): ",
                  i));

      int const result = sema.acquire ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  (result != 0 ? "fail\n" : "succeed\n")));
    }
}

struct Process_Semaphore_Options
{
  bool is_consumer {true};
  bool delete_semaphore {};
  int iterations {};
  int exit_delay {};
  ACE_TCHAR const *semaphore_name {ACE_TEXT ("Process_Semaphore_Test")};
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("csn:xi:d:"));
  //FUZZ: enable check_for_lack_ACE_OS

  Process_Semaphore_Options options;

  int opt {};

  //FUZZ: disable check_for_lack_ACE_OS
  while ((opt = getopt ()) != -1)
    {
  //FUZZ: enable check_for_lack_ACE_OS
      switch (opt)
        {
        case 'c':               // Make us a consumer.
          options.is_consumer = true;
          break;
        case 's':               // Make us a supplier.
          options.is_consumer = false;
          break;
        case 'x':               // Remove the semaphore after we're done.
          options.delete_semaphore = true;
          break;
        case 'n':               // Specify the name of the semaphore.
          options.semaphore_name = getopt.opt_arg ();
          break;
        case 'i':               // Number of acquire/release we'll perform.
          options.iterations = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'd':
          options.exit_delay = ACE_OS::atoi (getopt.opt_arg ());
          break;
        default:
          return -1;
        }
    }

  ACE_SYNCH_PROCESS_SEMAPHORE sema (0, options.semaphore_name);

  if (options.is_consumer)
    consumer (sema, options.iterations);
  else
    producer (sema, options.iterations);

  ACE_OS::sleep (options.exit_delay);

  if (options.delete_semaphore)
    sema.remove ();
  return 0;
}
