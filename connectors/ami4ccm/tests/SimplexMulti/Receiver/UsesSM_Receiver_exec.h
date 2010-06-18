// -*- C++ -*-
// $Id$

#ifndef CIAO_USESSM_RECEIVER_EXEC_H_
#define CIAO_USESSM_RECEIVER_EXEC_H_

#include "UsesSM_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "UsesSM_Receiver_exec_export.h"

namespace CIAO_UsesSM_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  class One_exec_i
    : public virtual ::UsesSM::CCM_One,
      public virtual ::CORBA::LocalObject
  {
  public:
    One_exec_i ( Atomic_UShort &nr_of_one_received);
    virtual ~One_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
    private:
      Atomic_UShort &nr_of_one_received_;
  };
  class Two_exec_i
    : public virtual ::UsesSM::CCM_Two,
      public virtual ::CORBA::LocalObject
  {
  public:
    Two_exec_i (Atomic_UShort &nr_of_two_received);
    virtual ~Two_exec_i (void);

    virtual void bar (::CORBA::Long cmd,
                      ::CORBA::String_out answer);
    private:
      Atomic_UShort &nr_of_two_received_;
  };
  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::UsesSM::CCM_One_ptr get_do_my_one (void);
    virtual ::UsesSM::CCM_Two_ptr get_do_my_two (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::UsesSM::CCM_Receiver_Context_var context_;
    Atomic_UShort nr_of_one_received_;
    Atomic_UShort nr_of_two_received_;
  };

  extern "C" USESSM_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_UsesSM_AMI_Receiver_Impl (void);
}
#endif /* ifndef */
