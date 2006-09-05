#ifndef COMMON_H
#define COMMON_H
#include "InterfaceC.h"
#include "ace/Vector_T.h"
#include "TargetManager/TargetManagerExtC.h"

namespace CIAO
{
  namespace RACE
  {
    /**
     * Application string classes.
     */
    enum Group {GUARANTEED, BEST_EFFORT};


    struct Component_Info
    {
      /// Component ID.
      CORBA::String_var id_;

      /// Name of the node where the component is deployed.
      CORBA::String_var node_;

      /// Reference to the NodeManager where the component is running.
      ::CIAO::NodeManagerDaemon_var node_mgr_;

    };


    struct App_String
    {
      /// Sstring "name".

      CORBA::String_var id_;

      /// Critical path.
      Critical_Path  critical_path_;

      /// Critical path deadline.
      CORBA::Double deadline_;

      /// Current e-2-e execution time of the critical path.
      CORBA::Double exec_time_;

      /// Plan ID obtained from the DeploymentPlan IDL.
      CORBA::String_var plan_id_;

      /// Plan label obtained from the DeploymentPlan IDL.
      CORBA::String_var label_;

      /// Relative importance.
      CORBA::Long importance_;

      /// String class.
      Group group_;

      /// OS priority at which the components of this app-string are
      /// executing.
      CORBA::Long priority_;

      /// ACE_Vector containing the Component_Info for all components of
      /// this app_string.
      ACE_Vector <Component_Info> components_;

      CORBA::Double miss_ratio_;

      CORBA::Long miss_count_;
    };

  }
}

#endif /*COMMON_H */
