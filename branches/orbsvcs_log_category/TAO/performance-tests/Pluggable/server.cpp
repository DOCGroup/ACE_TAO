// $Id$

#include "PP_Test_Server.h"
#include "tao/Timeprobe.h"

// This runs the server test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  PP_Test_Server pp_test_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tPluggable_Test: server\n\n"));


  try
    {
      int status = pp_test_server.init (argc,
                                        argv);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Initialization Error\n"),
                            -1);
        }
      else
        {
          pp_test_server.run ();
        }
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  ACE_TIMEPROBE_PRINT;

  return 0;
}
