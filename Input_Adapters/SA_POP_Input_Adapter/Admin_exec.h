#ifndef ADMIN_EXEC_H
#define ADMIN_EXEC_H

#include "SA_POP_Input_Adapter_svnt.h"
#include "SA_POP_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace SA_POP_IA
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        class SA_POP_INPUT_ADAPTER_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::SA_POP_IA::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i (SA_POP_IA_Component_Context *context);

          virtual ~Admin_exec_i (void);

          virtual ::CORBA::Boolean
            add_string (const ::CIAO::RACE::OperationalString & opstring,
                        char *& ID);

          virtual ::CORBA::Boolean
            delete_string (const char * ID)
            throw (UnknownID);

          virtual ::CORBA::Boolean
            deploy_string (const ::CIAO::RACE::OperationalString & opstring,
                           char *& ID);

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
          /// Object reference of the data base admin object.
          Data_Base::Admin_var DB_;

          /// Object reference of the DAnCE output adapter admin object.
          DAnCE_OA::Admin_var OA_;
        };
      }
    }
  }
}

#endif /* ADMIN_EXEC_H */
