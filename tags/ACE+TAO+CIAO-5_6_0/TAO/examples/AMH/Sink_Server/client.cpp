//
// $Id$
//

#include "Client_Task.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Client_Task task (argc, argv);

  if (task.parse_args () != 1)
    {

      ACE_OS::exit (1);
    }

  task.try_RT_scheduling ();

  if (task.narrow_servant () != 1)
    {
        ACE_OS::exit (1);
    }

  task.run_test();

  return 0;
}
