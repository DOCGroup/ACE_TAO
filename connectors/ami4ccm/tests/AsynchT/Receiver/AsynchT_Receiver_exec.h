// -*- C++ -*-
// $Id$

#ifndef CIAO_ASYNCHT_RECEIVER_EXEC_H_
#define CIAO_ASYNCHT_RECEIVER_EXEC_H_

#include "AsynchT_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "AsynchT_Receiver_exec_export.h"

namespace CIAO_AsynchT_Receiver_Impl
{
  class MyFoo_exec_i
    : public virtual ::AsynchT::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (void);
    virtual ~MyFoo_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
    virtual void bar (::CORBA::Long cmd, ::CORBA::Long_out l_cmd);
    virtual ::CORBA::Short rw_attrib ();
    virtual void rw_attrib (::CORBA::Short new_value);
    virtual CORBA::Short ro_attrib ();
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::AsynchT::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::AsynchT::CCM_Receiver_Context_var context_;
  };

  extern "C" ASYNCHT_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_AsynchT_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
