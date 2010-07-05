// -*- C++ -*-
// $Id$

#ifndef CIAO_INTERINOUT_T_RECEIVER_EXEC_H_
#define CIAO_INTERINOUT_T_RECEIVER_EXEC_H_

#include "InterInOutT_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "InterInOutT_Receiver_exec_export.h"

namespace CIAO_InterInOutT_Receiver_Impl
{
  class MyFoo_exec_i
    : public virtual ::InterInOutT::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (void);
    virtual ~MyFoo_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::Long &cmd,
      char * &answer);
    virtual void var_ins (const char * in_str, ::CORBA::String_out answer,
                          ::CORBA::Double_out cmd);
    virtual void var_div_ins (InterInOutT::TestTopic &test_topic,
                              InterInOutT::TopicString &topic_str,
                              InterInOutT::TestArray topic_arr,
                              char * & answer);
   virtual void var_div2_ins (InterInOutT::X_Union &topic_union,
                              InterInOutT::test_seq &seq,
                              CORBA::String_out answer);
   virtual void enum_in(CORBA::String_out answer,
                         InterInOutT::test_enum &in_test);
   };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::InterInOutT::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterInOutT::CCM_Receiver_Context_var context_;
  };

  extern "C" INTERINOUT_T_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterInOutT_AMI_Receiver_Impl (void);
}

#endif /* ifndef */

