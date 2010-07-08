// -*- C++ -*-
//
// $Id$

#ifndef CIAO_GETCOMP_T_SENDER_EXEC_H_
#define CIAO_GETCOMP_T_SENDER_EXEC_H_

#include "GetComp_SenderEC.h"
#include "GetComp_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_GetComp_Sender_Impl
{
  
  class MyFoo_callback_exec_i
    : public virtual ::GetComp::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);
    virtual int
    get_component(
      ::GetComp::CCM_Sender_Context_ptr context);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::GetComp::CCM_Sender_Context_var context_;
 };

  extern "C" GETCOMP_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_GetComp_AMI_Sender_Impl (void);
}

#endif /* ifndef */

