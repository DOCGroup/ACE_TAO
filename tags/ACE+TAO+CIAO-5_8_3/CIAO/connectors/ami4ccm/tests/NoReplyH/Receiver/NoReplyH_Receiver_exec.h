// -*- C++ -*-
// $Id$

#ifndef CIAO_NOREPLYH_RECEIVER_EXEC_H_
#define CIAO_NOREPLYH_RECEIVER_EXEC_H_

#include "NoReplyH_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "NoReplyH_Receiver_exec_export.h"

namespace CIAO_NoReplyH_Receiver_Impl
{

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  class MyFoo_exec_i
    : public virtual ::NoReplyH::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (Atomic_UShort &);
    virtual ~MyFoo_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::String_out answer);

    virtual void hello (::CORBA::Long_out answer);

    virtual ::CORBA::Short rw_attrib ();

    virtual void rw_attrib (::CORBA::Short new_value);

    virtual CORBA::Short ro_attrib ();
  private:
     CORBA::Long attrib_;
     Atomic_UShort &calls_;
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::NoReplyH::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::NoReplyH::CCM_Receiver_Context_var context_;
    Atomic_UShort calls_;
  };

  extern "C" NOREPLYH_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_NoReplyH_AMI_Receiver_Impl (void);
}

#endif /* ifndef */

