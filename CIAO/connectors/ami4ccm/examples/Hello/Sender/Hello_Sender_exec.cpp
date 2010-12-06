// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Sender_Impl
{
  void HandleException (
      long id,
      const char* error_string,
      const char* func)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Sender (%C) :\tCallback except from AMI.\n"),
                  func));
      if (id != 42)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("ERROR (%C) :\tReceived unexpected ID received")
                      ACE_TEXT(" in except handler.\n"),
                      func));
          return;
        }
      if (ACE_OS::strcmp (error_string, "Hello world") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("ERROR (%C) :\tReceived unexpected error string")
                      ACE_TEXT(" received in except handler.\n"),
                      func));
          return;
        }
    }

  void HandleException (
      ::CCM_AMI::ExceptionHolder_ptr excep_holder,
      const char* func)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Sender (%C) :\tHandle except AMI\n"), func));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const Hello::InternalError& ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Sender: Caught the correct except type")
                      ACE_TEXT(" <%u> <%C>\n"),
                      ex.id, ex.error_string.in ()));
          HandleException (ex.id, ex.error_string.in (), func);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("ERROR: Caught unexpected except:");
        }
    }

  //============================================================
  // Facet Executor Implementation Class: MyFoo_callback_exec_i
  //============================================================
  MyFoo_callback_exec_i::MyFoo_callback_exec_i (void)
  {
  }

  MyFoo_callback_exec_i::~MyFoo_callback_exec_i (void)
  {
  }

  //============================================================
  // Operations from ::CCM_AMI::MyFoo_callback
  //============================================================

  // FOO methods
  void
  MyFoo_callback_exec_i::foo (
    ::CORBA::Long ami_return_val,
    const char * answer)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (FOO) :\tCallback from AMI: result <%u> ")
                ACE_TEXT("answer <%C>\n"), ami_return_val, answer));
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "FOO");
  }

  // HELLO methods
  void
  MyFoo_callback_exec_i::hello (
    ::CORBA::Long answer)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (FOO) :\tCallback from AMI (HELLO) :")
                ACE_TEXT(" answer <%u>\n"), answer));
  }

  void
  MyFoo_callback_exec_i::hello_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "HELLO");
  }

  //GET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (RW_ATTRIB) :\tCallback from AMI (RW_ATTRIB):")
                ACE_TEXT(" answer <%u>\n"), ami_return_val));
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "GET RW_ATTRIB");
  }

  //SET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (RW_ATTRIB) :\tCallback from ")
                ACE_TEXT("AMI (RW_ATTRIB)\n")));
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "SET_RW_ATTRIB");
  }

  //ro_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (RO_ATTRIB) :\tCallback from AMI ")
                ACE_TEXT("(RO_ATTRIB) : answer <%u>\n"), ami_return_val));
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "RO_ATTRIB");
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::Hello::CCM_Sender_Context_ptr context)
  : context_(::Hello::CCM_Sender_Context::_duplicate (context))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    ::Hello::AMI4CCM_MyFoo_var my_foo_ami_  =
       context_->get_connection_sendc_run_my_foo();

    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("ERROR Sender (ASYNCH) :")
                    ACE_TEXT("\tfoo_ami is NIL !!!\n")));
        return 1;
      }
    ::Hello::CCM_AMI4CCM_MyFooReplyHandler_var cb =
        new MyFoo_callback_exec_i ();

    for (int i = 0; i < 5; ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (ASYNCH) :")
                    ACE_TEXT("\tInvoke Asynchronous calls\n")));
        my_foo_ami_->sendc_foo (cb.in (),
                                "Do something asynchronous");
        my_foo_ami_->sendc_hello (cb.in ());
        my_foo_ami_->sendc_get_rw_attrib(cb.in ());
        my_foo_ami_->sendc_get_rw_attrib(cb.in ());
        my_foo_ami_->sendc_set_rw_attrib(cb.in (), 15);
        my_foo_ami_->sendc_get_ro_attrib(cb.in ());
        my_foo_ami_->sendc_get_ro_attrib(cb.in ());
        ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("Sender (ASYNCH) :\tInvoked Asynchronous calls\n")));
      }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoke Asynchronous calls ")
                ACE_TEXT("to test except handling\n")));
    my_foo_ami_->sendc_foo (cb.in (), "");
    my_foo_ami_->sendc_set_rw_attrib(cb.in (), 0);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoked Asynchronous call.\n")));
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::Hello::CCM_Sender_Context_ptr context)
  : context_(::Hello::CCM_Sender_Context::_duplicate (context))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    ::Hello::MyFoo_var my_foo_ami_ =
         context_->get_connection_run_my_foo ();

    //run synch calls
    CORBA::String_var out_str;
    for (int i = 0; i < 5; ++i)
      {
        CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                               out_str.out ());
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call (FOO)")
                ACE_TEXT(" result <%u> answer <%C>\n"), result, out_str.in ()));

        CORBA::Long answer;
        my_foo_ami_->hello (answer);
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(HELLO) answer <%u>\n"), answer));

        try
          {
            CORBA::Short rw_attrib = my_foo_ami_->rw_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(GET_RW_ATTRIB) answer <%u>\n"), rw_attrib));
          }
        catch (const Hello::InternalError& ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH FOO) :\tExpected Except caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
          }

        try
          {
            my_foo_ami_->rw_attrib (15);
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(SET_RW_ATTRIB) to <15>\n")));
          }
        catch (const Hello::InternalError& ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH FOO) :\tExpected Except caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
          }

        try
          {
            CORBA::Short ro_attrib = my_foo_ami_->ro_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(GET_RO_ATTRIB) answer <%u>\n"), ro_attrib));
          }
        catch (const Hello::InternalError& ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH FOO) :\tExpected Except caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
          }
      }
    try
      {
        CORBA::Long result = my_foo_ami_->foo ("", out_str.out ());
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH) :\tInvoked synchronous call result ")
                ACE_TEXT("<%u> answer <%C>\n"), result, out_str.in ()));
      }
    catch (const Hello::InternalError& ex)
      {
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH FOO) :\tExpected Except caught : ")
                ACE_TEXT("<%u> <%C>\n"), ex.id, ex.error_string.in ()));
      }

    try
      {
        my_foo_ami_->rw_attrib (0);
        ACE_DEBUG ((LM_DEBUG,
           ACE_TEXT("Sender (SYNCH) :\tInvoked synchronous call rw_attrib\n")));
      }
    catch (const Hello::InternalError& ex)
      {
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH RW_ATTRIB) :\tExpected Except caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected except:");
      }
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.
  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->asynch_foo_gen_ =
      new asynch_foo_generator (this->context_.in ());
    this->asynch_foo_gen_->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    this->synch_foo_gen_ =
       new synch_foo_generator (this->context_.in());
    this->synch_foo_gen_->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    delete this->asynch_foo_gen_;
    this->asynch_foo_gen_ = 0;
    delete this->synch_foo_gen_;
    this->synch_foo_gen_ = 0;
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Sender_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}

