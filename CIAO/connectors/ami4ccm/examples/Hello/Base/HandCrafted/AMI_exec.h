// -*- C++ -*-
// $Id$

#ifndef CIAO_AMI_EXEC_H_
#define CIAO_AMI_EXEC_H_

#include "HelloS.h"
#include "HelloEC.h"
#include "Hello_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Hello_AMI_Sender_Impl
{
  class AMI_MyFoo_reply_handler : public POA_Hello::AMI_MyFooHandler
  {
    public:
      AMI_MyFoo_reply_handler (
      ::Hello::AMI_MyFooCallback_ptr foo_callback);

      virtual ~AMI_MyFoo_reply_handler (void);

      virtual void foo (CORBA::Long result, const char * out_str);

      virtual void foo_excep (::Messaging::ExceptionHolder * excep_holder);

      virtual void hello (CORBA::Long answer);

      virtual void hello_excep (::Messaging::ExceptionHolder * excep_holder);

      virtual void get_rw_attrib (::CORBA::Short ami_return_val);

      virtual void get_rw_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      virtual void set_rw_attrib ();

      virtual void set_rw_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      virtual void get_ro_attrib (::CORBA::Short ami_return_val);

      virtual void get_ro_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);
    private:
      ::Hello::AMI_MyFooCallback_var foo_callback_;
  };

  class AMI_MyFoo_exec_i
    : public virtual ::Hello::CCM_AMI_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_MyFoo_exec_i ();

    virtual ~AMI_MyFoo_exec_i (void);

    virtual void
    sendc_foo (
      ::Hello::AMI_MyFooCallback_ptr ami_handler,
      const char * in_str);

    virtual void
    sendc_hello (
      ::Hello::AMI_MyFooCallback_ptr ami_handler);

    virtual void
    sendc_get_rw_attrib (
      ::Hello::AMI_MyFooCallback_ptr ami_handler);

    virtual void
    sendc_set_rw_attrib (
      ::Hello::AMI_MyFooCallback_ptr ami_handler,
      CORBA::Short rw_attrib);

    virtual void
        sendc_get_ro_attrib (
      ::Hello::AMI_MyFooCallback_ptr ami_handler);

    void
    set_session_context (::Components::SessionContext_ptr ctx);

  private:
    ::Hello::MyFoo_var ami_foo_server_;
    ::Hello::CCM_AMI_CONN_MyFoo_Context_var context_;
  };

  class  AMI_exec_i
    : public virtual ::Hello::CCM_AMI_CONN_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_exec_i (void);
    virtual ~AMI_exec_i (void);

    virtual ::Hello::CCM_AMI_MyFoo_ptr
    get_provides_MyFoo (void);

    virtual void
    set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello::CCM_AMI_CONN_MyFoo_Context_var context_;
    ::Hello::AMI_MyFooCallback_var callback_foo_;
    ::Hello::MyFoo_var receiver_foo_;
    AMI_MyFoo_exec_i *myfoo_;
  };

  extern "C" HELLO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_AMI_Impl (void);
}

#endif /* ifndef */

