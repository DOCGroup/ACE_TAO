/**
 * @file CIAO_ComponentServer.cpp
 * @author William R. Otte
 * $Id$
 *
 * Implementation and main for CIAO_ComponentServer.
 */

#include "CIAO_ComponentServer_Task.h"
#include "ciao/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  // Tracing disabled by default
  CIAO_DISABLE_TRACE ();

  CIAO_TRACE ("CIAO_ComponentServer::ACE_TMAIN");

  int retval = 0;
  try
    {
      CIAO::Deployment::ComponentServer_Task cs (argc, argv);
      cs.run ();

      // Wait for the threads to exit.
      //ACE_Thread_Manager::instance ()->wait ();
    }
  catch (CIAO::Deployment::ComponentServer_Task::Error &e)
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_ComponentServer main: Caught ComponentServer exception: %C\n",
                  e.err_.c_str ()));
      retval = -1;
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_ComponentServer main: Caught unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

