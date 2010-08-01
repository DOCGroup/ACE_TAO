// -*- C++ -*-
//
// $Id$

#ifndef CIAO_INTERMULTI_SENDER_EXEC_H_
#define CIAO_INTERMULTI_SENDER_EXEC_H_

#include "InterMulti_SenderEC.h"
#include "InterMulti_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_InterMulti_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;
   
  /// Worker threads for asynchronous invocations
  class asynch_one_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_one_generator (::InterMulti::CCM_Sender_Context_ptr context);

    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };

  class asynch_two_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_two_generator (::InterMulti::CCM_Sender_Context_ptr context);

    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };

  class asynch_three_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_three_generator (::InterMulti::CCM_Sender_Context_ptr context);

    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };

  /// Worker threads for synchronous invocations
  class synch_one_generator : public virtual ACE_Task_Base
  {
  public:
    synch_one_generator (::InterMulti::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };
  class synch_two_generator : public virtual ACE_Task_Base
  {
  public:
    synch_two_generator (::InterMulti::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };
    class synch_three_generator : public virtual ACE_Task_Base
  {
  public:
    synch_three_generator (::InterMulti::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };

  class One_callback_exec_i
    : public virtual ::InterMulti::CCM_AMI4CCM_OneReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    One_callback_exec_i (void);
    virtual ~One_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
  };

  class Two_callback_exec_i
    : public virtual ::InterMulti::CCM_AMI4CCM_TwoReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    Two_callback_exec_i (void);
    virtual ~Two_callback_exec_i (void);

    virtual void bar (const char * answer);
    virtual void bar_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
  };

  class Three_callback_exec_i
    : public virtual ::InterMulti::CCM_AMI4CCM_ThreeReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    Three_callback_exec_i (void);
    virtual ~Three_callback_exec_i (void);

    virtual void plus (const char * answer);
    virtual void plus_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void bar (const char * answer);
    virtual void bar_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterMulti::CCM_Sender_Context_var context_;
  };

  extern "C" INTERMULTI_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterMulti_AMI_Sender_Impl (void);
}

#endif /* ifndef */
