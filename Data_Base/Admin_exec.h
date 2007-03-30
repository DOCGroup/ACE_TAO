#ifndef ADMIN_EXEC_H
#define ADMIN_EXEC_H

#include "Data_Base_svnt.h"
#include "Data_Base_exec_export.h"
#include "Data_Base_exec.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Data_Base
    {
      namespace CIDL_Data_Base_Component_Impl
      {

        class DATA_BASE_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::Data_Base::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:

          Admin_exec_i (Data_Base_Component_exec_i *executor);

          virtual ~Admin_exec_i (void);

          virtual ::CORBA::Boolean
          add_string (const ::CIAO::RACE::OperationalString & opstring);

          virtual void
          remove_string (const char * ID)
            throw (UnknownID);

          virtual void
          modify_string
            (const ::CIAO::RACE::OperationalString & opstring,
             const char * ID)
            throw (UnknownID);

          virtual void
          update_string_status (const char * ID, ::CORBA::Boolean active)
            throw (UnknownID);

          private:

          /// Pointer to the component executor object.
          Data_Base_Component_exec_i *db_executor_;

        };

      }
    }
  }
}
#endif /* ADMIN_EXEC_H */
