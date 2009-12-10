// -*- C++ -*-
//
// $Id$

#ifndef CIAO_AMI_EXEC_H_
#define CIAO_AMI_EXEC_H_

#include "AMIEC.h"
#include "AMI_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_Hello_AMI_AMI_AMI_Impl
{
  class AMI_perform_work : public ACE_Task_Base
  {
    public:
      AMI_perform_work (CORBA::ORB_ptr orb);
      virtual int svc (void);

    private:
      CORBA::ORB_var orb_;
  };

  class  AMI_MyFoo_exec_i
    : public virtual ::Hello_AMI::CCM_AMI_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_MyFoo_exec_i ();

    virtual ~AMI_MyFoo_exec_i (void);

    void provide_receiver (::Hello::MyFoo_ptr receiver_foo);

    virtual void
    sendc_foo (
      ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler,
      const char * in_str);

    virtual void
    sendc_hello (
      ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler);

    virtual void
    sendc_get_rw_attrib (
      ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler);
    virtual void
    sendc_set_rw_attrib (
      ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler,
      CORBA::Short rw_attrib);
    virtual void
        sendc_get_ro_attrib (
      ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler);

  private:
    Hello::MyFoo_var ami_foo_server_;
  };

  class  AMI_exec_i
    : public virtual AMI_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMI_exec_i (void);
    virtual ~AMI_exec_i (void);

    // Supported operations and attributes.

    // Component attributes.

    // Port operations.

    virtual ::Hello_AMI::CCM_AMI_MyFoo_ptr
    get_perform_asynch_my_foo (void);

    // Operations from Components::SessionComponent.

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello_AMI_AMI::CCM_AMI_Context_var  context_;
    ::Hello_AMI::AMI_MyFooCallback_var callback_foo_;
    ::Hello::MyFoo_var receiver_foo_;

    AMI_MyFoo_exec_i *myfoo_;
  };

  extern "C" AMI_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_AMI_Impl (void);
}

namespace CIAO_Hello_AMI_AMI_AMI_Impl
{
  class  AMIHome_exec_i
    : public virtual AMIHome_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    AMIHome_exec_i (void);

    virtual ~AMIHome_exec_i (void);

    // All operations and attributes.

    // Factory operations.

    // Finder operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  };

  extern "C" AMI_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_AMI_AMIHome_Impl (void);
}

#endif /* ifndef */

