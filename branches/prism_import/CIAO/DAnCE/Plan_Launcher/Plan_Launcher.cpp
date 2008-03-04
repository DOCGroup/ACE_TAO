// $Id$

#include "Plan_Launcher_Impl.h"

namespace DAnCE
  {
  namespace Plan_Launcher
    {


    int
    run_main_implementation (int argc, char *argv[])
    {
      try
        {
          Plan_Launcher_Impl pl (argc, argv);
          pl.execute();
        }
      catch (const Plan_Launcher_Impl::Deployment_Failure& e)
        {
          ACE_ERROR ( (LM_ERROR, "[%M] PlanLauncher error : %s.\n", e.error_.c_str()));
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Plan_Launcher.cpp::run_main_implementation ");
          return -1;
        }
      catch (...)
        {
          ACE_ERROR ( (LM_ERROR, "[%M] Plan_Launcher.cpp::run_main_implementation Unknown exception.\n"));
          return -1;
        }
      return 0;
    }


  } // Plan_Launcher
} // DAnCE




