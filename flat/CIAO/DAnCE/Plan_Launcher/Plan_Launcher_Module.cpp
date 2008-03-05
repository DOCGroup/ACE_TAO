// -*- C++ -*-
// $Id$

#include "Plan_Launcher_Module.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "Plan_Launcher_Impl.h"
#include "ace/Get_Opt.h"

ACE_RCSID (DAnCE,
           DAnCE_Plan_Launcher_Module,
           "$Id$")

using namespace DAnCE::Plan_Launcher;

DAnCE_Plan_Launcher_Module::DAnCE_Plan_Launcher_Module (void)
{
}

CORBA::Object_ptr
DAnCE_Plan_Launcher_Module::create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[])
{
  try
    {
      ACE_ERROR ( (LM_TRACE, "[%M] Plan_Launcher_Module init starting.\n"));
      Plan_Launcher_Impl pl (orb, argc, argv);
      pl.execute();
    }
  catch (const Plan_Launcher_Base_Impl::Deployment_Failure& e)
    {
      ACE_ERROR ( (LM_ERROR, "[%M] PlanLauncher error : %s.\n", e.error_.c_str()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Plan_Launcher_Module.cpp::init ");
    }
  catch (...)
    {
      ACE_ERROR ( (LM_ERROR, "[%M] Plan_Launcher_Module.cpp::init Unknown exception.\n"));
    }
  return CORBA::Object::_nil ();
}

ACE_FACTORY_DEFINE (DAnCE_Plan_Launcher_Module, DAnCE_Plan_Launcher_Module)


