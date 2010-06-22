// -*- C++ -*-
// $Id$

#include "Hello_Sender_comp_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Sender_Impl
{
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
  // Foo methods
  void
  MyFoo_callback_exec_i::foo (
    ::CORBA::Long ami_return_val,
    const char * answer)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from foo:")
                ACE_TEXT("result <%u> answer <%u>\n"),
                ami_return_val, answer));
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    /* Your code here. */
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Hello::InternalError& ex)
      {
         ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("Sender:\tMyFoo Expected excception AMI Callback ")
                 ACE_TEXT("from foo: id <%u> error_string <%C>\n"),
                 ex.id, ex.error_string.in ()));
      }
  }

  // Hello methods
  void
  MyFoo_callback_exec_i::hello (
      ::CORBA::Long answer)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from hello:")
                ACE_TEXT(" answer <%u>\n"), answer));
  }

  void
  MyFoo_callback_exec_i::hello_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    /* Your code here. */
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Hello::InternalError& ex)
      {
        ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo Excveption AMI Callback from hello: ")
                ACE_TEXT(" id <%u> error_string <%C>\n"),
                ex.id, ex.error_string.in ()));
      }
  }

  // GET rw_attrib meyhods
  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from get_rw_attrib:")
                ACE_TEXT(" result <%u>\n"), ami_return_val));
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    /* Your code here. */
    excep_holder->raise_exception ();
  }

  //SET rw_attrib methods
  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from ")
                ACE_TEXT("set_rw_attrib.\n")));
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    /* Your code here. */
    excep_holder->raise_exception ();
  }

  //ro_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from ")
                ACE_TEXT("get_ro_attrib: result <%u>\n"), ami_return_val));
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    /* Your code here. */   
    excep_holder->raise_exception ();
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
                           ::Hello::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::Hello::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (2);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :")
                ACE_TEXT("\tInvoke Asynchronous calls\n")));

    for (int i = 0; i < 5; ++i)
      {
        if (CORBA::is_nil (my_foo_ami_))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("ERROR Sender (ASYNCH) :")
                        ACE_TEXT("\tfoo_ami is NIL !!!\n")));
            return 1;
          }
        else
          {
            my_foo_ami_->sendc_foo (new MyFoo_callback_exec_i (),
                                    "Do something asynchronous");
            my_foo_ami_->sendc_hello (new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_get_rw_attrib(new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_set_rw_attrib(new MyFoo_callback_exec_i (), 15);
            my_foo_ami_->sendc_get_ro_attrib(new MyFoo_callback_exec_i ());
            ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT("Sender (ASYNCH) :\tInvoked 5 Asynchronous calls\n")));
          }
      }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoke Asynchronous calls ")
                ACE_TEXT("to test except handling\n")));
    my_foo_ami_->sendc_foo (new MyFoo_callback_exec_i (), "");
    my_foo_ami_->sendc_set_rw_attrib(new MyFoo_callback_exec_i (), 0);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoked 2 Asynchronous call.\n")));
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (::Hello::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::Hello::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (2);
    // run synch calls
    CORBA::String_var out_str;
    for (int i = 0; i < 5; ++i)
      {
        try
          {
            ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT("Sender (SYNCH) :\tInvoke synchronous call (foo)\n")));
            CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                   out_str.out());
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call (foo)")
                    ACE_TEXT(" result <%u> answer <%C>\n"), 
                             result, out_str.in()));

            CORBA::Long answer;
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH) :\tInvoke synchronous call")
                    ACE_TEXT(" (hello)\n")));
            my_foo_ami_->hello (answer);
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoked synchronous ")
                    ACE_TEXT("call  (hello) result <%u>\n"), answer));

            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH) :\tInvoke synchronous call ")
                    ACE_TEXT("(rw_attrib())\n")));
            CORBA::Short rw_attrib = my_foo_ami_->rw_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(rw_attrib()) return <%u>\n"), rw_attrib));

            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH) :\tInvoke synchronous call")
                ACE_TEXT(" (rw_attrib(15))\n")));
            my_foo_ami_->rw_attrib (15);
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(rw_attrib(15))\n")));
 
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH) :\tInvoke synchronous call")
                ACE_TEXT(" (ro_attrib())\n")));
            CORBA::Short ro_attrib = my_foo_ami_->ro_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(ro_attrib) return <%u>\n"), ro_attrib));
          }
        catch (const Hello::InternalError& ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH) :\tUnexpected Exception caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
           }
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
    ::Hello::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::Hello::MyFoo_var synch_foo =
        this->context_->get_connection_run_my_foo ();
    synch_foo_generator* synch_foo_gen =
      new synch_foo_generator (synch_foo);
    synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
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

