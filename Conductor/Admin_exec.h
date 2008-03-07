#ifndef ADMIN_EXEC_H
#define ADMIN_EXEC_H

#include "ConductorEC.h"
#include "Conductor_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Conductor
    {
      namespace CIDL_Conductor_Component_Impl
      {
        class CONDUCTOR_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::Conductor::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i
            (::CIAO::RACE::Conductor::CCM_Conductor_Component_Context_ptr
             context,
             Logger &logger);

          virtual ~Admin_exec_i (void);

          virtual ::CORBA::Boolean
            add_string (const ::CIAO::RACE::OperationalString & opstring,
                        ::CORBA::String_out ID);

          virtual ::CORBA::Boolean
            delete_string (const char * ID)
            throw (UnknownID);

          virtual ::CORBA::Boolean
            deploy_string (const ::CIAO::RACE::OperationalString & opstring,
                            ::CORBA::String_out ID);

          virtual ::CORBA::Boolean
            deploy_string_with_id (const char * ID)
            throw (UnknownID);

          virtual ::CORBA::Boolean
            modify_string (const ::CIAO::RACE::OperationalString & opstring,
                           const char * ID)
            throw (UnknownID);

          virtual ::CORBA::Boolean tear_down_string (const char * ID)
            throw (UnknownID);

          virtual ::CORBA::Boolean init_system ();

          virtual ::CORBA::Boolean start_controller();

          virtual ::CORBA::Boolean stop_controller();

          private:
          /// Object reference of the data base admin object.
          Data_Base::Admin_var DB_;

          /// Object reference of the DAnCE output adapter admin object.
          DAnCE_OA::Admin_var OA_;

          /// Object reference of the Controller's Admin object.
          Controller::Admin_var Controller_Admin_;

          /// Object reference of the Controller's Control_Operations object.
          Controller::Control_Operations_var Controller_Ops_;

          Logger &logger_;
        };
      }
    }
  }
}

#endif /* ADMIN_EXEC_H */
