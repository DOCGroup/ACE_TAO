#ifndef QUERY_EXEC_H
#define QUERY_EXEC_H

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
        class DATA_BASE_EXEC_Export Query_exec_i
          : public virtual ::CIAO::RACE::Data_Base::CCM_Query,
            public virtual TAO_Local_RefCounted_Object
        {
          public:

          Query_exec_i (Data_Base_Component_exec_i *executor);

          virtual ~Query_exec_i (void);

          virtual ::CIAO::RACE::OperationalString *
          get_string_by_id (const char * id)
            throw (UnknownID);

          virtual ::CIAO::RACE::UUIDs *
            get_all_id (const ::CIAO::RACE::TimeValue & tv);

          virtual ::CIAO::RACE::UUIDs *
          get_all_active_id (const ::CIAO::RACE::TimeValue & tv);

          virtual ::CIAO::RACE::UUIDs *
          get_modified_id (const ::CIAO::RACE::TimeValue & tv);

          private:

          /// Pointer to the component executor object.
          Data_Base_Component_exec_i *db_executor_;

        };
      }
    }
  }
}

#endif /* QUERY_EXEC_H */
