// -*- C++ -*-
// $Id$

#ifndef CIAO_AMI_EXEC_H_
#define CIAO_AMI_EXEC_H_

#include "AMIEC.h"
#include "AMI_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Hello_AMI_Sender_Impl
{
  class  AMI_MyFoo_exec_i
    : public virtual ::Hello::CCM_AMI_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_MyFoo_exec_i ();

    virtual ~AMI_MyFoo_exec_i (void);

    void provide_receiver (::Hello::MyFoo_ptr receiver_foo);

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

  private:
    Hello::MyFoo_var ami_foo_server_;
  };

  class  AMI_exec_i
    : public virtual AMI_Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_exec_i (void);
    virtual ~AMI_exec_i (void);

    virtual ::Hello::CCM_AMI_MyFoo_ptr
    get_sendc_run_my_foo (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello::CCM_AMI_Sender_Context_var context_;
    ::Hello::AMI_MyFooCallback_var callback_foo_;
    ::Hello::MyFoo_var receiver_foo_;

    AMI_MyFoo_exec_i *myfoo_;
  };

  extern "C" AMI_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_AMI_Impl (void);
}

#endif /* ifndef */

