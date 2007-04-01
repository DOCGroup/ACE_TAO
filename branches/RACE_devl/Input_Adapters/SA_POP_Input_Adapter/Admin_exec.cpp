#include "Admin_exec.h"

namespace CIAO
{
  namespace RACE
  {
    namespace SA_POP_IA
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        Admin_exec_i::Admin_exec_i (SA_POP_IA_Component_Context *context)
        {
          try
            {
              this->DB_ = context->get_connection_DB ();
              this->OA_ = context->get_connection_OA ();
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught::Admin_exec:: "
                                   "Error while resolving references to "
                                   "DAnCE OA & RACE Data Base objects!\n");
            }
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Admin_exec_i::add_string (
          const ::CIAO::RACE::OperationalString & opstring,
          char *& ID)
        {
          try
            {
              this->DB_->add_string (opstring);
              ID = ::CORBA::string_dup (opstring.ID.in ());
              return true;
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in "
              "SA_POP_IA::Admin_exec_i:add_string.\n");
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::delete_string (const char * ID)
          throw (UnknownID)
        {
          try
            {
              this->DB_->remove_string (ID);
              return true;
            }
          catch (UnknownID &ex)
            {
              ACE_ERROR ((LM_ERROR, "Given string ID is not know to RACE. ID:%s\n",
                          ID));
              throw ex;
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string (const ::CIAO::RACE::OperationalString & opstring,
          char *& ID)
        {
          try
            {
              if (this->OA_->deploy_string (opstring))
                {
                  this->DB_->add_string (opstring);
                  ID = ::CORBA::string_dup (opstring.ID.in ());
                  return true;
                }
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (ex, "Exception caught in "
                                   "SA_POP_IA::Admin_exec_i::deploy_string.\n");
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
              ACE_ERROR ((LM_ERROR, "Given string ID is not know to RACE. ID:%s\n",
                          ID));
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
              if (this->OA_->deploy_string (opstring))
                {
                  this->DB_->modify_string (opstring, ID);
                  return true;
                }
            }
          catch (UnknownID &ex)
            {
              ACE_ERROR ((LM_ERROR, "Given string ID is not know to RACE. ID:%s\n",
                          ID));
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
              ACE_ERROR ((LM_ERROR, "Given string ID is not know to RACE. ID:%s\n",
                          ID));
              throw ex;
            }
          return false;
        }
      }
    }
  }
}
