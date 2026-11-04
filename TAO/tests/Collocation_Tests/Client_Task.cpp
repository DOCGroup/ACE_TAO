
#include "Client_Task.h"
#include "TestC.h"

Client_Task::Client_Task (ACE_TCHAR const *ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (corb))
{
}

int
Client_Task::svc ()
{
  try
    {
      CORBA::Object_var const tmp {this->corb_->string_to_object (this->input_.c_str ())};

      Test::Hello_var const hello {Test::Hello::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             this->input_.c_str ()),
                           1);
        }

      CORBA::String_var const the_string {hello->get_string ()};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
