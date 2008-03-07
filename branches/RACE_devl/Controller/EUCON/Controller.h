#ifndef EUCON_CONTROLLER_H
#define EUCON_CONTROLLER_H

#include "EUCON_ControllerEC.h"
#include <vector>

#include "ace/Task.h"
#include "ace/Hash_Map_Manager_T.h"

#include "Logger.h"
#include "Task.h"
#include "MPC_Controller.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      class Controller : public ACE_Task_Base
      {
      public:

        Controller (::CIAO::RACE::Controller::CCM_EUCON_Controller_Context_ptr
                    context,
                    ::CORBA::Long interval);

        virtual ~Controller ();

        virtual bool init_controller ();

        virtual bool start_controller ();

        virtual bool stop_controller ();

        virtual ::CORBA::Boolean
        register_string (
          const ::CIAO::RACE::OperationalString & opstring,
          ::CORBA::String_out ID);

        virtual ::CORBA::Boolean
        unregister_string (
          const char * ID);

        virtual int svc ();

      protected:

        // Helper methods.

        virtual void control_action ();

        virtual bool populate_domain_info
          (const ::Deployment::Domain& domain);

        virtual void populate_node
        (const ::Deployment::Node &d_node,
         ::CIAO::RACE::Node &r_node);

        virtual void dump_task (::CIAO::RACE::Task task);

        virtual void populate_task
        (const ::CIAO::RACE::OperationalString & opstring,
         ::CIAO::RACE::Task & task);

        virtual int node_ref (const char *node);



      private:

        std::vector<::CIAO::RACE::Task> tasks_;

        CIAO::RACE::Domain domain_;

        MPC_Controller controller_;

        /// Context object reference.
        ::CIAO::RACE::Controller::CCM_EUCON_Controller_Context_var context_;

        /// System monitor object reference.
        ::CIAO::RACE::TM_Proxy::SystemSnapshot_var system_monitor_;

        /// System utility object reference.
        ::CIAO::RACE::TM_Proxy::Utility_var system_utility_;

        /// Reference to the application actuator.
        ::CIAO::RACE::Effector::ApplicationActuator_var appActuator_;

        /// Sampling period.
        ACE_Time_Value interval_;

        /// Is the controller active?
        bool active_;

        bool initialized_;

        ::CIAO::RACE::OperationalStrings opstrings_;

        Logger logger_;

        Logger util_logger_;

        Logger rate_logger_;

      };
    }
  }
}

#endif /* EUCON_CONTROLLER_H */
