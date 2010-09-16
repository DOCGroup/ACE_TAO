// -*- C++ -*-
//
// $Id$

#ifndef CIAO_THREECOMP_SENDER_EXEC_H_
#define CIAO_THREECOMP_SENDER_EXEC_H_

#include "ThreeComp_SenderEC.h"
#include "ThreeComp_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_ThreeComp_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::ThreeComp::CCM_Sender_Context_ptr context);

    virtual int svc (void);

  private:
    ::ThreeComp::CCM_Sender_Context_var context_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::ThreeComp::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::ThreeComp::CCM_Sender_Context_var context_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::ThreeComp::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);

  };
  class State_exec_i
        : public virtual ::ThreeComp::CCM_State,
          public virtual ::CORBA::LocalObject
      {
      public:
        State_exec_i (void);
        virtual ~State_exec_i (void);

        virtual CORBA::Long bar (::CORBA::Long cmd, ::CORBA::String_out);
   };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);
    virtual ::ThreeComp::CCM_State_ptr get_do_my_state (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::ThreeComp::CCM_Sender_Context_var context_;
  };

  extern "C" THREECOMP_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Sender_Impl (void);
}

#endif /* ifndef */
