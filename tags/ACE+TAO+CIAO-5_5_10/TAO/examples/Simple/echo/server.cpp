// $Id$
#include "../Simple_util.h"
#include "Echo_i.h"

// This is the main driver program for the time and date server.

int
main (int argc, char *argv[])
{
  Server<Echo_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tEcho server\n\n"));


  try
    {
      if (server.init ("Echo",
                       argc,
                       argv) == -1)
        return 1;
      else
        {
          server.run ();
        }
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }


  return 0;
}

