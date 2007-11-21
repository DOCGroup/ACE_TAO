#include "Admin_exec.h"
#include <string>

namespace CIAO
{
  namespace RACE
  {
    namespace Conductor
    {
      namespace CIDL_Conductor_Component_Impl
      {
        Admin_exec_i::Admin_exec_i (
          ::CIAO::RACE::Conductor::CCM_Conductor_Component_Context_ptr context,
          Logger &logger)
          : logger_ (logger)
        {
          try
            {
              this->DB_ = context->get_connection_DB ();
              this->OA_ = context->get_connection_OA ();
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught::Admin_exec:: "
                                "Error while resolving references to "
                                "DAnCE OA & RACE Data Base objects!\n";
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
              this->DB_->add_string (opstring);
              ID = ::CORBA::string_dup (opstring.ID.in ());
              return true;
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught in "
                                "Admin_exec_i:add_string.\n";
              msg += ex._info ().c_str();
              this->logger_.log (msg);

            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::delete_string (const char * ID)
          throw (UnknownID)
        {
          // @@ TODO: We are currently not handling the case where the
          // string has been deployed. Should we tear it down in addition
          // to deleting it from the database?
          try
            {
              this->DB_->remove_string (ID);
              return true;
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
              if (this->OA_->deploy_string (opstring))
                {
                  this->DB_->add_string (opstring);
                  return true;
                }
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
              ::CIAO::RACE::OperationalString_var opstring =
                this->DB_->get_string_by_id (ID);
              if (this->OA_->deploy_string (opstring.in ()))
                {
                  this->DB_->update_string_status (ID, true);
                  return true;
                }
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
          // @@ TODO: Now that we have ReDAC working, we need to redeploy
          // the string if it has ben already deployed in addition to
          // modifying it in the database.
          try
            {
              if (this->OA_->deploy_string (opstring))
                {
                  this->DB_->modify_string (opstring, ID);
                  return true;
                }
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
              this->DB_->remove_string (ID);
              this->OA_->tear_down_string (ID);
              return true;
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
