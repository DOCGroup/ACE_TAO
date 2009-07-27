#include "Admin_exec.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        Admin_exec_i::Admin_exec_i (CIAO_CIAO_RACE_Input_Adapter_SA_POP_IA_Component_Impl::SA_POP_IA_Component_Context *context,
                                    Logger &logger)
          : logger_ (logger)
        {
          try
            {
              this->conductor_ = context->get_connection_conductor ();
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught::Admin_exec:: "
                                "Error while resolving references to "
                                "RACE Conductor object!\n";
              msg += ex._info ().c_str();
              this->logger_.log (msg);

            }
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::add_string (
          const ::CIAO::RACE::OperationalString & opstring,
          ::CORBA::String_out ID)
        {
          try
            {
              return this->conductor_->add_string (opstring, ID);
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught in Admin_exec_i::"
                                "add_string.\n";
              msg += ex._info ().c_str();
              this->logger_.log (msg);

            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::delete_string (const char * ID)
          throw (UnknownID)
        {
          try
            {
              return this->conductor_->delete_string (ID);
            }
          catch (UnknownID &ex)
            {
              std::string msg = "Given string ID is not know to RACE. ID:";
              msg += ID;
              this->logger_.log (msg);
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string (
          const ::CIAO::RACE::OperationalString & opstring,
          ::CORBA::String_out ID)
        {
          try
            {
              return this->conductor_->deploy_string (opstring, ID);
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught in "
                                "SA_POP_IA::Admin_exec_i::deploy_string.\n";
              msg += ex._info ().c_str();
              this->logger_.log (msg);
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string_with_id (const char *ID)
          throw (UnknownID)
        {
          try
            {
              return this->deploy_string_with_id (ID);
            }
          catch (UnknownID &ex)
            {
              std::string msg = "Given string ID is not know to RACE. ID:";
              msg += ID;
              this->logger_.log (msg);
              throw ex;
            }
          return false;
        }

        CORBA::Boolean
        Admin_exec_i::modify_string
        (const ::CIAO::RACE::OperationalString & opstring,
         const char * ID) throw (UnknownID)
        {
          try
            {
              return this->conductor_->modify_string(opstring, ID);
            }
          catch (UnknownID &ex)
            {
              std::string msg = "Given string ID is not know to RACE. ID:";
              msg += ID;
              this->logger_.log (msg);
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::tear_down_string (const char * ID)
          throw (UnknownID)
        {
          try
            {
              return this->conductor_->tear_down_string (ID);
            }
          catch (UnknownID &ex)
            {
              std::string msg = "Given string ID is not know to RACE. ID:";
              msg += ID;
              this->logger_.log (msg);
              throw ex;
            }
          return false;
        }
      }
    }
  }
}
