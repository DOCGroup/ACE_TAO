// $Id$

#include "ace/Dynamic_Service.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/Logger/Logger_Service.h"
#include "ExecutionManager_Module.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  DANCE_DISABLE_TRACE ();

  int retval = 0;

  try
    {
      DAnCE::Logger_Service
        * dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      DANCE_TRACE_LOG (DANCE_LOG_TRACE,
                       (LM_TRACE, DLINFO
                        ACE_TEXT("ExecutionManager - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_TRACE_LOG (DANCE_LOG_TRACE,
                       (LM_TRACE, DLINFO
                        ACE_TEXT("ExecutionManager - initializing module instance\n")));

      DAnCE_ExecutionManager_Module em;
      CORBA::Object_var obj = em.init (orb.in (), argc, argv);

      if (!CORBA::is_nil (obj.in ()))
        {
          DANCE_TRACE_LOG (DANCE_LOG_TRACE,
                           (LM_TRACE, DLINFO
                            ACE_TEXT("ExecutionManager - running ORB\n")));
          orb->run ();
        }

      DANCE_TRACE_LOG (DANCE_LOG_TRACE,
                       (LM_TRACE, DLINFO
                        ACE_TEXT("ExecutionManager - destroying ORB\n")));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      DANCE_ERROR (DANCE_LOG_EMERGENCY,
                   (LM_EMERGENCY, DLINFO
                    "ExecutionManager - Caught CORBA Exception: %C\n",
                    ex._info ().c_str ()));
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (DANCE_LOG_EMERGENCY,
                   (LM_ERROR,
                    "ExecutionManager - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

