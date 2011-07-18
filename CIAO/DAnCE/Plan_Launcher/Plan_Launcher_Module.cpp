// -*- C++ -*-
// $Id$

#include "Plan_Launcher_Module.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "Plan_Launcher_Impl.h"
#include "ace/Get_Opt.h"
#include "DAnCE/Logger/Log_Macros.h"

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
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_Plan_Launcher_Module::create_object - "
                    "Creating Plan Launcher object."));
                    
      Plan_Launcher_Impl pl (orb, argc, argv);
      pl.execute();
    }
  catch (const Plan_Launcher_Base_Impl::Deployment_Failure& e)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_Plan_Launcher_Module::create_object - "
                     "Error : %s.\n", e.error_.c_str()));
    }
  catch (const CORBA::Exception& ex)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_Plan_Launcher_Module::create_object - "
                     "Caught CORBA Exception %C",
                     ex._info ().c_str ()));
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_Plan_Launcher_Module::create_object - "
                    "Unknown exception.\n"));
    }
  return CORBA::Object::_nil ();
}

ACE_FACTORY_DEFINE (DAnCE_Plan_Launcher_Module, DAnCE_Plan_Launcher_Module)


