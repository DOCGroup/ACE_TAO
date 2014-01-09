// $Id$
#include "Simple_util.h"
#include "Server_i.h"

// This is the main driver program for the time and date server.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Server<Checkpoint_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tCheckpoint Event_Types server\n\n"));

  try
    {
      if (server.init ("Event_Types_Checkpoint", argc, argv) == -1)
        return 1;
      else
        {
          server.run ();
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

