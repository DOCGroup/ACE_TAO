//
// $Id$
//

#include "Client_Task.h"
#include "TestC.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (const char *ior,
                          const int named_orbs,
                          int argc,
                          char **argv,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , name_ ()
  , argc_ (argc)
  , argv_ (argv)
{
  if (named_orbs)
    {
      this->name_.set ("Client_ORB");
    }
}

int
Client_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         this->name_.c_str ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var tmp =
        orb->string_to_object (input_
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                            1);
        }

      CORBA::String_var the_string =
        hello->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;

}
