// -*- C++ -*-
// $Id$

#ifndef CIAO_EXCEPTION_T_RECEIVER_EXEC_H_
#define CIAO_EXCEPTION_T_RECEIVER_EXEC_H_

#include "ExceptionT_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ExceptionT_Receiver_exec_export.h"

namespace CIAO_ExceptionT_Receiver_Impl
{
  class MyFoo_exec_i
    : public virtual ::ExceptionT::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (void);
    virtual ~MyFoo_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::String_out answer);

    virtual void hello (::CORBA::Long_out answer);

    virtual ::CORBA::Short rw_attrib ();

    virtual void rw_attrib (::CORBA::Short new_value);

    virtual CORBA::Short ro_attrib ();
  private:
    bool get_rw_;
    bool get_ro_;
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::ExceptionT::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::ExceptionT::CCM_Receiver_Context_var context_;
  };

  extern "C" EXCEPTION_T_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ExceptionT_AMI_Receiver_Impl (void);
}

#endif /* ifndef */

