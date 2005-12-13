//
// $Id$
//

#include "Client_Task.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Client_Task task (convert.get_argc(), convert.get_ASCII_argv());

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
