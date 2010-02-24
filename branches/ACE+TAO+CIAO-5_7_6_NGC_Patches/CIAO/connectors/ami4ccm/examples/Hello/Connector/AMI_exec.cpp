// -*- C++ -*-
// $Id$

#include "AMI_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "AMI_MyFoo_i.h"

namespace CIAO_Hello_AMI_Sender_Impl
{
  AMI_MyFoo_exec_i::AMI_MyFoo_exec_i ()
  {
  }

  AMI_MyFoo_exec_i::~AMI_MyFoo_exec_i (void)
  {
  }

  void
  AMI_MyFoo_exec_i::provide_receiver (::Hello::MyFoo_ptr receiver_foo)
  {
    this->ami_foo_server_ = ::Hello::MyFoo::_duplicate (receiver_foo);
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
            ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
              new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
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
            ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
              new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
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
            ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
              new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
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
            ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
              new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
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
            ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
              new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
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
  AMI_exec_i::get_sendc_run_my_foo (void)
  {
    return this->myfoo_;
  }
  // Operations from Components::SessionComponent.

  void
  AMI_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_AMI_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  AMI_exec_i::configuration_complete (void)
  {
    this->receiver_foo_ =
      this->context_->get_connection_run_my_foo ();

    this->myfoo_->provide_receiver (receiver_foo_.in ());
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
