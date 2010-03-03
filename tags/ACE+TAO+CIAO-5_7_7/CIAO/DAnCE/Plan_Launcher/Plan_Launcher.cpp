// $Id$

#include "ace/Dynamic_Service.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "Plan_Launcher_Impl.h"

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
                    ACE_TEXT("PlanLauncher - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                   ACE_TEXT("PlanLauncher - initializing module instance\n")));

      Plan_Launcher_Impl pl (orb.in (), argc, argv);
      pl.execute();

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("PlanLauncher - destroying ORB\n")));

      orb->destroy ();
    }
  catch (const Plan_Launcher_Impl::Deployment_Failure& e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO "PlanLauncher - Error: %C.\n", e.error_.c_str()));
      retval = -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("PlanLauncher");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "PlanLauncher - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

