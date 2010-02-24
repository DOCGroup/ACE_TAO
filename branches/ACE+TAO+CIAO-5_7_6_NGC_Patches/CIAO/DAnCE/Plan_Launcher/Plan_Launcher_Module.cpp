// -*- C++ -*-
// $Id$

#include "Plan_Launcher_Module.h"
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
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE_Plan_Launcher_Module::create_object - ")
                    ACE_TEXT("Creating Plan Launcher object.\n")));

      Plan_Launcher_Impl pl (orb, argc, argv);
      pl.execute();
    }
  catch (const Plan_Launcher_Base_Impl::Help_Issued& )
    {
    }
  catch (const Plan_Launcher_Base_Impl::Deployment_Failure& e)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                   ACE_TEXT("DAnCE_Plan_Launcher_Module::create_object - ")
                   ACE_TEXT("Error : %C.\n"), e.error_.c_str()));
      throw;
    }
  catch (const CORBA::Exception& ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                   ACE_TEXT("DAnCE_Plan_Launcher_Module::create_object - ")
                   ACE_TEXT("Caught CORBA Exception %C\n"),
                   ex._info ().c_str ()));
      throw;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                   ACE_TEXT("DAnCE_Plan_Launcher_Module::create_object - ")
                   ACE_TEXT("Unknown exception.\n")));
      throw;
    }
  return CORBA::Object::_nil ();
}

ACE_FACTORY_DEFINE (DAnCE_Plan_Launcher_Module, DAnCE_Plan_Launcher_Module)


