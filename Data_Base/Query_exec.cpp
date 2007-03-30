#include "Query_exec.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Data_Base
    {
      namespace CIDL_Data_Base_Component_Impl
      {
        Query_exec_i::Query_exec_i (Data_Base_Component_exec_i *executor)
          : db_executor_ (executor)
        {
        }

        Query_exec_i::~Query_exec_i (void)
        {
        }

        ::CIAO::RACE::OperationalString *
        Query_exec_i::get_string_by_id (const char * ID)
          throw (UnknownID)
        {
          ::CIAO::RACE::OperationalString opstring;
          int ret = this->db_executor_->data_base_.find (ID, opstring);

          if (ret == 0)
            {
              ::CIAO::RACE::OperationalString_var ret_val =
                new ::CIAO::RACE::OperationalString (opstring);
              return ret_val._retn ();
            }
          else
            {
              UnknownID ex;
              ex.ID = ID;
              ex.reason = "The given ID has not been registered with the data base.";
              throw ex;
            }
        }

        ::CIAO::RACE::UUIDs *
        Query_exec_i::get_all_id (const ::CIAO::RACE::TimeValue & /* tv */)
        {

          ::CIAO::RACE::UUIDs_var ids = new ::CIAO::RACE::UUIDs ();

          for (DATA_BASE_ITER itr = this->db_executor_->data_base_.begin ();
               itr != this->db_executor_->data_base_.end ();
               ++itr)
            {
              ids->length (ids->length () + 1);
              (*ids) [ids->length () - 1] = (*itr).ext_id_;
            }
          return ids._retn ();
        }

        ::CIAO::RACE::UUIDs *
        Query_exec_i::get_all_active_id (const ::CIAO::RACE::TimeValue & /* tv */)
        {
          // Your code here.
          return 0;
        }

        ::CIAO::RACE::UUIDs *
        Query_exec_i::get_modified_id (const ::CIAO::RACE::TimeValue & /* tv */)
        {
          // Your code here.
          return 0;
        }

      }
    }
  }
}
