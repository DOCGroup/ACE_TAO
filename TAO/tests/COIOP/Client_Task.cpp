// -*- C++ -*-
#include "Client_Task.h"
#include "tao/ORB_Core.h"
#include "TestC.h"

Client_Task::Client_Task (std::basic_string<ACE_TCHAR> const &ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *const thr_mgr,
                          bool const result)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (corb))
  , result_ (result)
{
}

int
Client_Task::svc ()
{
  try
    {
      CORBA::Object_var const tmp {
        this->corb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (this->input_.c_str ())) };

      Test::Hello_var const hello {Test::Hello::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             this->input_.c_str ()),
                            1);
        }

      CORBA::String_var const the_string {hello->get_string ()};

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      if (!this->result_)
        {
          // We would expect the call to fail, so we have an error now
          ACE_ERROR ((LM_ERROR, "(%P|%t) - ERROR: get_string should have failed\n"));
        }

      hello->shutdown ();
    }
  catch (CORBA::TRANSIENT const &ex)
    {
      if (!this->result_)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected exception\n"));
          // When collocation has been disabled we expect a transient
          // with minor code 2
          return 0;
        }

      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
