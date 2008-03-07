#include "Admin_exec.h"
#include <sstream>

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
              this->Controller_Admin_ =
                context->get_connection_controllerAdmin();
              this->Controller_Ops_ =
                context->get_connection_controlOperations();
            }
          catch (CORBA::Exception &ex)
            {
              std::string msg = "Exception caught::Admin_exec:: "
                                "Error while resolving references to "
                                "DAnCE_OA/RACE_Data_Base/"
                                "Controller_Admin/Controlelr_Ops "
                                "objects!\n";
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
          std::stringstream msg;
          msg << "Admin_exec_i::deploy_string:";
          msg << "Trying to deploy string " << opstring.ID << ".\n";
          try
            {
              if (this->OA_->deploy_string (opstring))
              {
                msg << "Successfully deployed string. "
                  << "Now trying to register with the controller\n";
                if (this->Controller_Ops_->register_string (opstring, ID))
                {
                  msg << "Successfully resitered with the controller.\n";
                }
                else
                {
                  msg << "Oops! Error while registering with "
                      << "the controller.\n";
                }
                //this->DB_->add_string (opstring);
                this->logger_.log (msg.str());
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
          try
            {
              if (this->OA_->redeploy_string (opstring))
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
          std::stringstream msg;
          msg << "In Admin_exec_i::tear_down_string with ID = "
              << ID
              << std::endl;
          try
            {
                msg << "Trying to unregister the string with the controller\n";
                if (this->Controller_Ops_->unregister_string (ID))
                {
                  msg << "Successfully unresitered the string "
                      << "with the controller.\n";
                }
                else
                {
                  msg << "Oops! Error while unregistering the string with "
                      << "the controller.\n";
                }

                // this->DB_->remove_string (ID);
                msg << "Now removing the string form the system....";
                this->OA_->tear_down_string (ID);
                msg << "done!\n";
                this->logger_.log (msg.str ());
                return true;
            }
          catch (UnknownID &ex)
            {
              msg << "Given string ID is not know to RACE!\n";
              this->logger_.log (msg.str ());
              throw ex;
            }
        }

        ::CORBA::Boolean
        Admin_exec_i::init_system ()
        {
          std::stringstream msg;
          msg << "Admin_exec_1::init_system:"
              << "Trying to initialize the controller.\n";
          if (this->Controller_Admin_->init_controller())
          {
            msg << "Successfully initialized the controller.\n";
            this->logger_.log(msg.str());
            return true;
          }
          else
          {
            msg << "Error while initializing the controller.\n";
            this->logger_.log(msg.str());
            return false;
          }
        }

        ::CORBA::Boolean
        Admin_exec_i::start_controller ()
        {
          std::stringstream msg;
          msg << "Admin_exec_1::start_controller:"
              << "Trying to start the controller.\n";
          if (this->Controller_Admin_->start_controller())
          {
            msg << "Successfully started the controller.\n";
            this->logger_.log(msg.str());
            return true;
          }
          else
          {
            msg << "Error while starting the controller.\n";
            this->logger_.log(msg.str());
            return false;
          }
        }

        ::CORBA::Boolean
        Admin_exec_i::stop_controller ()
        {
          std::stringstream msg;
          msg << "Admin_exec_1::stop_controller:"
              << "Trying to stop the controller.\n";
          if (this->Controller_Admin_->stop_controller())
          {
            msg << "Successfully stopped the controller.\n";
            this->logger_.log(msg.str());
            return true;
          }
          else
          {
            msg << "Error while stopping the controller.\n";
            this->logger_.log(msg.str());
            return false;
          }
        }
      }
    }
  }
}
