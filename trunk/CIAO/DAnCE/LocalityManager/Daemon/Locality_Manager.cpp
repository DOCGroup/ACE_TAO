/**
 * @file Locality_Manager.cpp
 * @author William R. Otte
 * $Id$
 *
 * Implementation and main for the DAnCE LocalityManager
 */

#include "Locality_Manager_Task.h"
#include "DAnCE/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  // Tracing disabled by default
  DANCE_DISABLE_TRACE ();

  DANCE_TRACE ("LocalityManager::ACE_TMAIN");

  int retval = 0;
  try
    {
      DAnCE::LocalityManager_Task cs (argc, argv);
      cs.run ();

      // Wait for the threads to exit.
      //ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const DAnCE::LocalityManager_Task::Error &e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO 
                       ACE_TEXT ("LocalityManager main: Caught ComponentServer exception: %C\n"),
                  e.err_.c_str ()));
      retval = -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("LocalityManager main: Caught corba exception:");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO 
                       ACE_TEXT ("LocalityManager main: Caught unknown exception.\n")));
      retval = -1;
    }

  return retval;
}

