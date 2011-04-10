/**
 * @file EM_Launcher.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Plan Launcher bridge for the Locality Manager.
 */

#ifndef DANCE_PLAN_LAUNCHER_LM
#define DANCE_PLAN_LAUNCHER_LM

#include "Plan_Launcher_Base_Impl_T.h"

#include "dance/DAnCE_LocalityManagerC.h"
#include "dance/Plan_Launcher/Plan_Launcher_Export.h"

namespace DAnCE
{
  class DAnCE_Plan_Launcher_Export LM_Launcher :
    public virtual Plan_Launcher_Base_Impl< ::DAnCE::LocalityManager,
                                            ::DAnCE::LocalityManager,
                                            ::DAnCE::LocalityManager >
  {
    typedef Plan_Launcher_Base_Impl< ::DAnCE::LocalityManager,
      ::DAnCE::LocalityManager,
      ::DAnCE::LocalityManager > __Base;

  public:
    LM_Launcher (CORBA::ORB_ptr orb,
                 __Base::Manager_ptr managerw);

    ~LM_Launcher (void);


    bool lookup_by_uuid (const char *uuid,
                         CORBA::Object_out am,
                         CORBA::Object_out app);
  };
}

#endif
