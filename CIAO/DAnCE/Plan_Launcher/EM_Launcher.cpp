// $Id$

#include "EM_Launcher.h"

namespace DAnCE
{
  EM_Launcher::EM_Launcher (CORBA::ORB_ptr orb,
                                      __Base::Manager_ptr manager)
    : __Base (orb, manager)
  {
  }
    

  
  EM_Launcher::~EM_Launcher (void)
  {
    
  }
  
  bool
  EM_Launcher::lookup_by_uuid (const char *uuid,
                               CORBA::Object_out am,
                               CORBA::Object_out app)
  {
    DANCE_TRACE ("EM_Launcher::lookup_by_uuid");
    
    try
      {
        ::Deployment::DomainApplicationManagers_var managers = this->manager_->getManagers ();
        
        for (CORBA::ULong i = 0; i < managers->length (); ++i)
          {
            ::Deployment::DeploymentPlan_var plan (managers[i]->getPlan ());
            ACE_CString plan_uuid (plan->UUID.in ());
            if (plan_uuid == uuid)
              {
                am = CORBA::Object::_duplicate (managers[i]);
                
                ::Deployment::Applications_var apps = managers[i]->getApplications ();
                
                if (apps->length () != 0)
                  {
                    app = CORBA::Object::_duplicate (apps[0]);
                    return true;
                  }
                else return false;
              }
          }
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("EM_Launcher::lookup_by_uuid - ")
                         ACE_TEXT ("Caught CORBA Exception: %C\n"),
                         ex._info ().c_str ()));
        throw Deployment_Failure (ex._info ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("EM_Launcher::lookup_by_uuid - ")
                         ACE_TEXT ("Unknown C++ exception\n")));
        throw Deployment_Failure ("Unknown C++ exception in lookup_by_uuid\n");
      }
    
    return false;
  }
  
}
