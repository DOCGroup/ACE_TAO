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
              ACE_PRINT_EXCEPTION (ex, "Exception caught in SA_POP_IA::Admin_exec_i:\n");
            }
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::delete_string (const char * /* ID */)
        {
          // Your code here.
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string (const ::CIAO::RACE::OperationalString & /* opstring */,
          char *& /* ID */)
        {
          // Your code here.
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::deploy_string_with_id
        (const char * /* ID */)
        {
          // Your code here.
          return false;
        }

        CORBA::Boolean
        Admin_exec_i::modify_string
        (const ::CIAO::RACE::OperationalString & /* opstring */,
         const char * /* ID */)
        {
          // Your code here.
          return false;
        }

        ::CORBA::Boolean
        Admin_exec_i::tear_down_string (
          const char * /* ID */
        )
        {
          // Your code here.
          return false;
        }
      }
    }
  }
}
