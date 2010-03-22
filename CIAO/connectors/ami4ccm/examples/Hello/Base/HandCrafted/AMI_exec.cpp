// -*- C++ -*-
// $Id$

#include "AMI_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "connectors/ami4ccm/ami4ccm/ami4ccm.h"

namespace CIAO_Hello_AMI_Sender_Impl
{
  AMI_MyFoo_reply_handler::AMI_MyFoo_reply_handler (::Hello::AMI_MyFooCallback_ptr foo_callback)
  : foo_callback_ (::Hello::AMI_MyFooCallback::_duplicate (foo_callback))
  {
  }

  AMI_MyFoo_reply_handler::~AMI_MyFoo_reply_handler ()
  {
  }

  // FOO methods
  void
  AMI_MyFoo_reply_handler::foo (
    CORBA::Long result,
    const char * out_str)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo\n");
    this->foo_callback_->foo (result, out_str);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::foo_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->foo_excep (&holder);
    this->_remove_ref ();
  }

  // HELLO methods
  void
  AMI_MyFoo_reply_handler::hello (
    CORBA::Long answer)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello\n");
    this->foo_callback_->hello (answer);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::hello_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->hello_excep (&holder);
    this->_remove_ref ();
  }

  //GET rw_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_atrrib\n");
    this->foo_callback_->get_rw_attrib (ami_return_val);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::get_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->get_rw_attrib_excep (&holder);
    this->_remove_ref ();
  }

  //SET rw_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::set_rw_attrib ()
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib\n");
    this->foo_callback_->set_rw_attrib ();
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::set_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->set_rw_attrib_excep (&holder);
    this->_remove_ref ();
  }

  //ro_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_attrib\n");
    this->foo_callback_->get_ro_attrib (ami_return_val);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::get_ro_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->get_ro_attrib_excep (&holder);
    this->_remove_ref ();
  }
  AMI_MyFoo_exec_i::AMI_MyFoo_exec_i ()
  {
  }

  AMI_MyFoo_exec_i::~AMI_MyFoo_exec_i (void)
  {
  }

  void
  AMI_MyFoo_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_AMI_CONN_MyFoo_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    this->ami_foo_server_ = this->context_->get_connection_uses_MyFoo ();
  }

  void
  AMI_MyFoo_exec_i::sendc_foo (
    ::Hello::AMI_MyFooCallback_ptr ami_handler,
    const char * in_str)
  {
    printf ("AMI (FOO) :\tsendc_foo <%s>\n", in_str);
    if (!::CORBA::is_nil (this->ami_foo_server_.in ()))
      {
        Hello::AMI_MyFooHandler_var the_handler_var;
        if (!::CORBA::is_nil (ami_handler))
          {
            AMI_MyFoo_reply_handler*  handler =
              new AMI_MyFoo_reply_handler (ami_handler);
            the_handler_var = handler->_this ();
          }
        printf ("AMI (FOO) :\tSending string <%s> to AMI CORBA server\n", in_str);
        this->ami_foo_server_->sendc_foo (the_handler_var.in (), in_str);
        printf ("AMI (FOO) : \tInvoked sendc_foo\n");
      }
  }

  void
  AMI_MyFoo_exec_i::sendc_hello (
    ::Hello::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_hello\n");
    if (!::CORBA::is_nil (this->ami_foo_server_.in ()))
      {
        Hello::AMI_MyFooHandler_var the_handler_var;
        if (!::CORBA::is_nil (ami_handler))
          {
            AMI_MyFoo_reply_handler*  handler =
              new AMI_MyFoo_reply_handler (ami_handler);
            the_handler_var = handler->_this ();
          }
        printf ("AMI (FOO) :\tCalling AMI CORBA server\n");
        this->ami_foo_server_->sendc_hello (the_handler_var.in ());
        printf ("AMI (FOO) : \tInvoked sendc_foo\n");
      }
  }

  void
  AMI_MyFoo_exec_i::sendc_get_rw_attrib (
    ::Hello::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_get_rw_attrib\n");
    if (!::CORBA::is_nil (this->ami_foo_server_.in ()))
      {
        Hello::AMI_MyFooHandler_var the_handler_var;
        if (!::CORBA::is_nil (ami_handler))
          {
            AMI_MyFoo_reply_handler*  handler =
              new AMI_MyFoo_reply_handler (ami_handler);
            the_handler_var = handler->_this ();
          }
        this->ami_foo_server_->sendc_get_rw_attrib (the_handler_var.in ());
        printf ("AMI (FOO) : \tInvoked sendc_get_rw_attrib\n");
      }
  }

  void
  AMI_MyFoo_exec_i::sendc_set_rw_attrib (
    ::Hello::AMI_MyFooCallback_ptr ami_handler,
    CORBA::Short rw_attrib)
  {
    printf ("AMI (FOO) :\tsendc_set_rw_attrib\n");
    if (!::CORBA::is_nil (this->ami_foo_server_.in ()))
      {
        Hello::AMI_MyFooHandler_var the_handler_var;
        if (!::CORBA::is_nil (ami_handler))
          {
            AMI_MyFoo_reply_handler*  handler =
              new AMI_MyFoo_reply_handler (ami_handler);
            the_handler_var = handler->_this ();
          }
        printf ("AMI (FOO) : \tSet rw_attrib <%d>\n", rw_attrib);
        this->ami_foo_server_->sendc_set_rw_attrib (the_handler_var.in (), rw_attrib);
        printf ("AMI (FOO) : \tInvoked sendc_set_rw_attrib\n");
      }
  }

  void
  AMI_MyFoo_exec_i::sendc_get_ro_attrib (
    ::Hello::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_get_ro_attrib\n");
    if (!::CORBA::is_nil (this->ami_foo_server_.in ()))
      {
        Hello::AMI_MyFooHandler_var the_handler_var;
        if (!::CORBA::is_nil (ami_handler))
          {
            AMI_MyFoo_reply_handler*  handler =
              new AMI_MyFoo_reply_handler (ami_handler);
            the_handler_var = handler->_this ();
          }
        this->ami_foo_server_->sendc_get_ro_attrib (the_handler_var.in ());
        printf ("AMI (FOO) : \tInvoked sendc_get_ro_attrib\n");
      }
  }
  //============================================================
  // Component Executor Implementation Class: AMI_exec_i
  //============================================================

  AMI_exec_i::AMI_exec_i (void)
    : myfoo_ (new AMI_MyFoo_exec_i ())
  {
  }

  AMI_exec_i::~AMI_exec_i (void)
  {
    delete myfoo_;
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.

  ::Hello::CCM_AMI_MyFoo_ptr
  AMI_exec_i::get_provides_MyFoo (void)
  {
    return this->myfoo_;
  }
  // Operations from Components::SessionComponent.

  void
  AMI_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_AMI_CONN_MyFoo_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    this->myfoo_->set_session_context (this->context_.in ());
  }

  void
  AMI_exec_i::configuration_complete (void)
  {
  }

  void
  AMI_exec_i::ccm_activate (void)
  {
  }

  void
  AMI_exec_i::ccm_passivate (void)
  {
  }

  void
  AMI_exec_i::ccm_remove (void)
  {
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_AMI_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      AMI_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}
