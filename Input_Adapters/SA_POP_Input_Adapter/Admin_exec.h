#ifndef ADMIN_EXEC_H
#define ADMIN_EXEC_H

#include "SA_POP_Input_Adapter_svnt.h"
#include "SA_POP_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"
#include "utils/Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        class SA_POP_INPUT_ADAPTER_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::Input_Adapter::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i (CIAO_CIAO_RACE_Input_Adapter_SA_POP_IA_Component_Impl::SA_POP_IA_Component_Context *context,
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

          private:
          /// Object reference of the conductor object.
          Conductor::Admin_var conductor_;

          Logger &logger_;
        };
      }
    }
  }
}

#endif /* ADMIN_EXEC_H */
