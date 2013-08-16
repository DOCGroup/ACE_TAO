// $Id$
#include "../Simple_util.h"
#include "Grid_i.h"

// This is the main driver program for the time and date server.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Server<Grid_Factory_i> server;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\tGrid server\n\n")));

  try
    {
      if (server.init ("Grid",
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
      //ACE_UNUSED_ARG (userex);
      userex._tao_print_exception ("User Exception in main");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      //ACE_UNUSED_ARG (sysex);
      sysex._tao_print_exception ("System Exception in main ");
      return -1;
    }
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception ("CORBA exception in main");
      return 1;
    }

  return 0;
}
