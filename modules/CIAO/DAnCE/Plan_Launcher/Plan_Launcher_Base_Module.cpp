// -*- C++ -*-
// $Id$

#include "Plan_Launcher_Base_Module.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "ace/Get_Opt.h"
#include "Plan_Launcher_Base_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"

ACE_RCSID (DAnCE,
           DAnCE_Plan_Launcher_Base_Module,
           "$Id$")

using namespace DAnCE::Plan_Launcher;

DAnCE_Plan_Launcher_Base_Module::DAnCE_Plan_Launcher_Base_Module (void)
{
}

CORBA::Object_ptr
DAnCE_Plan_Launcher_Base_Module::create_object (CORBA::ORB_ptr orb,
                                                int argc,
                                                ACE_TCHAR *argv[])
{
  try
    {
      Plan_Launcher_Base_Impl pl (orb, argc, argv);
      pl.execute();
    }
  catch (const Plan_Launcher_Base_Impl::Deployment_Failure& e)
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] PlanLauncher error : %s.\n", e.error_.c_str()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Plan_Launcher_Base_Module.cpp::run_main_implementation ");
    }
  catch (...)
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] Plan_Launcher_Base_Module.cpp::run_main_implementation Unknown exception.\n"));
    }
  return CORBA::Object::_nil ();
}


ACE_FACTORY_DEFINE (DAnCE_Plan_Launcher_Base_Module, DAnCE_Plan_Launcher_Base_Module)
