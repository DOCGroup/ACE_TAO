// $Id$
#include "../Simple_util.h"
#include "Echo_i.h"

// This is the main driver program for the time and date server.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Server<Echo_i> server;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\tEcho server\n\n")));

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
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception ("CORBA exception");
      return 1;
    }

  return 0;
}
