// -*- C++ -*-
// $Id$

#ifndef CIAO_THREECOMP_RECEIVER_EXEC_H_
#define CIAO_THREECOMP_RECEIVER_EXEC_H_

#include "ThreeComp_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ThreeComp_Receiver_exec_export.h"

namespace CIAO_ThreeComp_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  class MyFoo_exec_i
    : public virtual ::ThreeComp::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (Atomic_UShort &nr_of_received);
    virtual ~MyFoo_exec_i (void);

    virtual CORBA::Long foo (::CORBA::Long cmd, ::CORBA::String_out answer);
  private:
    Atomic_UShort &nr_of_received_;
   };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::ThreeComp::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::ThreeComp::CCM_Receiver_Context_var context_;
    Atomic_UShort nr_of_received_;
  };

  extern "C" THREECOMP_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
