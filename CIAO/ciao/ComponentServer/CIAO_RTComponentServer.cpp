/**
 * @file CIAO_RTComponentServer.cpp
 *
 * $Id$
 *
 * Implementation and main for CIAO_RTComponentServer.
 */

#include "CIAO_RTComponentServer_Task.h"
#include "ace/Thread_Manager.h"
#include "ciao/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  // Tracing disabled by default
  CIAO_DISABLE_TRACE ();

  CIAO_TRACE ("CIAO_RTComponentServer::ACE_TMAIN");

  int retval = 0;
  try
    {
      CIAO::Deployment::RTComponentServer_Task cs (argc, argv);
      cs.run ();

      // Wait for the threads to exit.
      //ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const CIAO::Deployment::RTComponentServer_Task::Error &e)
    {
      CIAO_DEBUG (6, (LM_ALERT, CLINFO "CIAO_RTComponentServer main: Caught ComponentServer exception: %C\n",
                  e.err_.c_str ()));
      retval = -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CIAO_RTComponentServer main: Caught corba exception:");
      retval = -1;
    }
  catch (...)
    {
      CIAO_DEBUG (6, (LM_ALERT, CLINFO "CIAO_RTComponentServer main: Caught unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

