// $Id$

#include "ace/Process_Semaphore.h"
#include "ace/Synch_T.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

int producer (ACE_SYNCH_PROCESS_SEMAPHORE &sema,
              int iter)
{
  for (int i = iter; i > 0; --i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Try acquiring the semaphore (%d): ",
                  i));

      int result = sema.release ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  (result != 0 ? "fail\n" : "succeed\n")));
    }
  return 0;
}

int consumer (ACE_SYNCH_PROCESS_SEMAPHORE &sema,
              int iter)
{
  for (int i = iter; i > 0; --i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Try acquiring the semaphore (%d): ",
                  i));

      int result = sema.acquire ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  (result != 0 ? "fail\n" : "succeed\n")));
    }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("csn:xi:d:"));

  int is_consumer = 1;          // By default, make us a consumer.
  int delete_sema = 0;
  int iteration = 0;
  int exit_delay = 0;
  const ACE_TCHAR *sema_name = ACE_TEXT ("Process_Semaphore_Test");

  int opt;

  while ((opt = getopt ()) != -1)
    {
      switch (opt)
        {
        case 'c':               // Make us a consumer.
          is_consumer = 1;
          break;
        case 's':               // Make us a supplier.
          is_consumer = 0;
          break;
        case 'x':               // Remove the semaphore after we're done.
          delete_sema = 1;
          break;
        case 'n':               // Specify the name of the semaphore.
          sema_name = getopt.opt_arg ();
          break;
        case 'i':               // Number of acquire/release we'll perform.
          iteration = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'd':
          exit_delay = ACE_OS::atoi (getopt.opt_arg ());
          break;
        default:
          return -1;
        }
    };

  ACE_SYNCH_PROCESS_SEMAPHORE sema (0, sema_name);

  if (is_consumer != 0)
    consumer (sema, iteration);
  else
    producer (sema, iteration);

  ACE_OS::sleep (exit_delay);

  if (delete_sema != 0)
    sema.remove ();
  return 0;
}
