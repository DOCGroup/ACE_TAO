#include "Admin_exec.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Data_Base
    {
      namespace CIDL_Data_Base_Component_Impl
      {
        Admin_exec_i::Admin_exec_i (Data_Base_Component_exec_i *executor)
          : db_executor_ (executor)
        {
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::add_string
        (const ::CIAO::RACE::OperationalString & opstring)
        {
          int ret = this->db_executor_->data_base_.bind (opstring.ID.in (), opstring);

          if (ret == 0)
            {
              return true;
            }
          else if (ret == 1)
            {
              ACE_ERROR ((LM_ERROR, "RACE::Data_Base::Admin_exec_i: "
                          "A string with UUID: %s has been already registered! "
                          "No modifications to the data base is being made.\n",
                          opstring.ID.in ()));
            }
          return false;
        }

        void
        Admin_exec_i::remove_string (const char *ID)
          throw (UnknownID)
        {
          int ret = this->db_executor_->data_base_.unbind (ID);

          if (ret != 0)
            {
              UnknownID ex;
              ex.ID = ID;
              ex.reason = "The given ID has not been registered with the data base.";
              throw ex;
            }

        }

        void
        Admin_exec_i::modify_string (const ::CIAO::RACE::OperationalString &opstring,
                                     const char * ID)
          throw (UnknownID)
        {
          int ret = this->db_executor_->data_base_.find (ID);
          if (ret == 0)
            {
              this->db_executor_->data_base_.rebind (ID, opstring);
            }
          else
            {
              UnknownID ex;
              ex.ID = ID;
              ex.reason = "The given ID has not been registered with the data base.";
              throw ex;
            }
        }

        void
        Admin_exec_i::update_string_status (const char * ID,
                                            ::CORBA::Boolean active)
          throw (UnknownID)
        {
          ::CIAO::RACE::OperationalString opstring;
          int ret = this->db_executor_->data_base_.find (ID, opstring);

          if (ret == 0)
            {
              opstring.active = active;
              this->db_executor_->data_base_.rebind (ID, opstring);
            }
          else
            {
              UnknownID ex;
              ex.ID = ID;
              ex.reason = "The given ID has not been registered with the data base.";
              throw ex;
            }
        }
      }
    }
  }
}
