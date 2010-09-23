// $Id$

#include "ace/Dynamic_Service.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/Logger/Logger_Service.h"
#include "Target_Manager_Module.h"

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

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("TargetManager - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                   ACE_TEXT("TargetManager - initializing module instance\n")));

      DAnCE_TargetManager_Module tm;
      CORBA::Object_var obj = tm.init (orb.in (), argc, argv);

      if (!CORBA::is_nil (obj.in ()))
        {
          DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("TargetManager - running ORB\n")));
          orb->run ();
        }

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("TargetManager - destroying ORB\n")));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TargetManager");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "TargetManager - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

