/**
 * @file NM_Launcher.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Plan Launcher bridge for the Node Manager.
 */

#ifndef DANCE_PLAN_LAUNCHER_NM
#define DANCE_PLAN_LAUNCHER_NM

#include "Plan_Launcher_Base_Impl.h"

#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerC.h"
#include "DAnCE/Deployment/Deployment_NodeManagerC.h"

namespace DAnCE
{
  class NM_Launcher : 
    public virtual Plan_Launcher_Base_Impl< ::Deployment::NodeManager,
                                            ::Deployment::NodeApplicationManager,
                                            ::Deployment::NodeApplication >
  {
    typedef Plan_Launcher_Base_Impl< ::Deployment::NodeManager,
                                     ::Deployment::NodeApplicationManager,
                                     ::Deployment::NodeApplication > __Base;
    
  public:
    NM_Launcher (CORBA::ORB_ptr orb,
                      __Base::Manager_ptr managerw);
    
    ~NM_Launcher (void);
    
    
  };
}

#endif
