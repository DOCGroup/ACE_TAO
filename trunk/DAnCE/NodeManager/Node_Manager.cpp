// $Id$

#include "ace/Dynamic_Service.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "ciao/Logger/Logger_Service.h"
#include "Node_Manager_Module.h"

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

      // Also initialize CIAO logger since we reuse parts of CIAO in the nodemanager
      CIAO::Logger_Service
        * clf = ACE_Dynamic_Service<CIAO::Logger_Service>::instance ("CIAO_Logger");

      if (clf)
        {
          clf->init (argc, argv);
        }
        
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("NodeManager - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                   ACE_TEXT("NodeManager - initializing module instance\n")));

      DAnCE_NodeManager_Module nm;
      CORBA::Object_var obj = nm.init (orb.in (), argc, argv);

      if (!CORBA::is_nil (obj.in ()))
        {
          DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("NodeManager - running ORB\n")));
          orb->run ();
        }

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("NodeManager - destroying ORB\n")));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NodeManager");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "NodeManager - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

