// -*- C++ -*-
//
// $Id$

#ifndef CIAO_INTERRETURN_T_SENDER_EXEC_H_
#define CIAO_INTERRETURN_T_SENDER_EXEC_H_

#include "InterReturnT_SenderEC.h"
#include "InterReturnT_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_InterReturnT_Sender_Impl
{
  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::InterReturnT::AMI4CCM_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterReturnT::AMI4CCM_MyFoo_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::InterReturnT::MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterReturnT::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::InterReturnT::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void ret_void (const char * answer, CORBA::Long l_cmd);
    virtual void ret_void_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_long (::CORBA::Long ami_return_val, 
                           const char * answer, CORBA::Long l_cmd);
    virtual void ret_long_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_double (::CORBA::Double ami_return_val,
                              const char * answer, CORBA::Long l_cmd);
    virtual void ret_double_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_struct (const InterReturnT::TestStruct &ami_return_val,
                            const char *  answer, CORBA::Long l_cmd);
    virtual void ret_struct_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_array (const InterReturnT::TestArr ami_return_val[],
                            const char * answer, CORBA::Long l_cmd);
    virtual void ret_array_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual  void ret_seq (const InterReturnT::TestSeq &ami_return_val,
                           const char *  answer, CORBA::Long l_cmd);
    virtual void ret_seq_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_union (const InterReturnT::X_Union &ami_return_val,
                            const char * answer, CORBA::Long l_cmd);
    virtual void ret_union_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void ret_enum (InterReturnT::test_enum ami_return_val,
                            const char * answer, CORBA::Long l_cmd);
    virtual void ret_enum_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);
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
    ::InterReturnT::CCM_Sender_Context_var context_;
  };

  extern "C" INTERRETURN_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterReturnT_AMI_Sender_Impl (void);
}

#endif /* ifndef */
