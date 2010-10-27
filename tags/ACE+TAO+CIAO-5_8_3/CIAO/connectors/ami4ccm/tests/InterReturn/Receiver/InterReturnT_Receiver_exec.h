// -*- C++ -*-
// $Id$

#ifndef CIAO_INTERRETURN_T_RECEIVER_EXEC_H_
#define CIAO_INTERRETURN_T_RECEIVER_EXEC_H_

#include "InterReturnT_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "InterReturnT_Receiver_exec_export.h"

namespace CIAO_InterReturnT_Receiver_Impl
{
  class MyFoo_exec_i
    : public virtual ::InterReturnT::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (void);
    virtual ~MyFoo_exec_i (void);
    virtual void ret_void (const char * in_str,
                           ::CORBA::String_out answer,
                           ::CORBA::Long_out l_cmd );
    virtual CORBA::Long ret_long (const char * in_str,
                             ::CORBA::String_out answer,
                             ::CORBA::Long_out l_cmd);
    virtual CORBA::Double ret_double (const char * in_str,
                             ::CORBA::String_out answer,
                             ::CORBA::Long_out l_cmd);
    virtual InterReturnT::TestStruct *ret_struct ( const char * in_str,
                             ::CORBA::String_out answer,
                             ::CORBA::Long_out l_cmd);
    virtual InterReturnT::TestArray_slice *ret_array (const char * in_str,
                             ::CORBA::String_out answer,
                             ::CORBA::Long_out l_cmd);
    virtual InterReturnT::TestSeq *ret_seq (const char * in_str,
                             ::CORBA::String_out,
                             ::CORBA::Long_out l_cmd);
    virtual InterReturnT::X_Union *ret_union (const char * in_str,
                             ::CORBA::String_out,
                             ::CORBA::Long_out l_cmd);
    virtual InterReturnT::test_enum ret_enum (const char * in_str,
                             ::CORBA::String_out,
                             ::CORBA::Long_out l_cmd);
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::InterReturnT::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterReturnT::CCM_Receiver_Context_var context_;
  };

  extern "C" INTERRETURN_T_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterReturnT_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
