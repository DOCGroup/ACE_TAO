// $Id$

#include "Clerk_i.h"
#include "ace/OS_main.h"



// This is the main driver program for the Time Service clerk.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  Clerk_i clerk;

  ACE_DEBUG ((LM_DEBUG,
              "[SERVER] Process/Thread Id : (%P/%t) Time Service clerk\n"));

  try
    {
      int r = clerk.init (argc, argv);
      if (r == -1)
        return 1;
      else
        {
          clerk.run ();
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
