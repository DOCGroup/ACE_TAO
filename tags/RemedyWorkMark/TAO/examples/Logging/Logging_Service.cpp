// $Id$

#include "Logging_Service_i.h"
#include "tao/debug.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked
  TAO_debug_level = 0;

  Logger_Server log_server;

  try
    {
      int ret = log_server.init (argc, argv);
      if (ret == -1)
        return 1;
      else
        {
          log_server.run ();
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

  return 0;
}
