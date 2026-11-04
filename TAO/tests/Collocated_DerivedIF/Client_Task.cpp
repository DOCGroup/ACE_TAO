#include "Client_Task.h"
#include "TestC.h"

#include <cstring>

Client_Task::Client_Task (std::basic_string<ACE_TCHAR> const &ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *const thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (corb))
{
}

int
Client_Task::svc ()
{
  bool failed {};

  try
    {
      CORBA::Object_var const tmp {
        this->corb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (this->input_.c_str ()))};

      Test::HelloAgain_var const hello_again {Test::HelloAgain::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello_again.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::HelloAgain reference <%s>\n"),
                             this->input_.c_str ()),
                            1);
        }

      CORBA::String_var the_string {hello_again->get_string ()};
      if (std::strcmp (the_string.in (), "Hello there!") != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) - Error, incorrect string returned <%C>\n",
                      the_string.in ()));
          failed = true;
        }
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string from hello_again returned <%C>\n",
                  the_string.in ()));
      the_string = hello_again->get_string_again ();

      if (std::strcmp (the_string.in (), "Bye!") != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) - Error, incorrect string returned <%C>\n",
                      the_string.in ()));
          failed = true;
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string  from hello_again returned <%C>\n",
                  the_string.in ()));
      hello_again->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      failed = true;
    }

  return failed;
}
