// -*- C++ -*-
// $Id$

// test the use of one replyhandler with more then one request
#include "GetComp_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_GetComp_Sender_Impl
{
  CORBA::Boolean GetComponent = false;
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
    ::CORBA::Long /* ami_return_val*/,
    const char * /*answer*/)
  {
  }

  void
  MyFoo_callback_exec_i::foo_excep (::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
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
  // Operations from Components::SessionComponent.
  int
  Sender_exec_i::get_component(::GetComp::CCM_Sender_Context_ptr context)
  {
    ACE_OS::sleep (3);

    ::GetComp::AMI4CCM_MyFoo_var my_foo_ami_ =
      context_->get_connection_sendc_run_my_foo();

    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR Sender (ASYNCH)")
                              ACE_TEXT (" :\tmy_foo_ami is NIL !\n")));
        return 1;
      }

    CORBA::Object_var cmp;
    try
      {
        cmp = my_foo_ami_->_get_component ();
        if (::CORBA::is_nil (cmp.in ()))
        {
          GetComp::InternalError ex (1, "Unable to get component interface");
          throw ex;
        }
      }
    catch (const GetComp::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Sender:")
                    ACE_TEXT ("caught InternalError exception: retval <%C>\n"),
                    ex.error_string.in()));
        return 1;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected except:");
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Sender :")
                    ACE_TEXT ("Unable to get component interface\n")));
        return 1;
      }

    try
      {
        ::GetComp::AMI4CCM_MyFoo_Connector::AMI4CCM_Connector_var conn =
          ::GetComp::AMI4CCM_MyFoo_Connector::AMI4CCM_Connector::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            GetComp::InternalError ex (1, "Narrowed connector is nil.\n");
            throw ex;
          }
       }
     catch (const GetComp::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Sender:")
                    ACE_TEXT ("caught InternalError exception: retval <%C>\n"),
                    ex.error_string.in()));
        return 1;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected except:");
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Sender:")
                    ACE_TEXT ("Unable to narrow connector interface\n")));
        return 1;
      }
    return 0;
  }

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::GetComp::CCM_Sender_Context::_narrow (ctx);
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
    if (get_component(this->context_.in())== 0)
      {
        GetComponent = true;
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (GetComponent != true)
      {
        ACE_ERROR ((LM_ERROR, 
                    ACE_TEXT ("ERROR:")
                    ACE_TEXT (" Sender not possible to get component\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OK: Sender, possible to get component \n")));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_GetComp_AMI_Sender_Impl (void)
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
