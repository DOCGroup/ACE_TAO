// -*- C++ -*-
// $Id$

#ifndef CIAO_USESMULTI_RECEIVER_EXEC_H_
#define CIAO_USESMULTI_RECEIVER_EXEC_H_

#include "UsesMulti_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "UsesMulti_Receiver_exec_export.h"

namespace CIAO_UsesMulti_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  class One_exec_i
    : public virtual ::UsesMulti::CCM_One,
      public virtual ::CORBA::LocalObject
  {
  public:
    One_exec_i ( Atomic_UShort &nr_of_received);
    virtual ~One_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
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

    virtual ::UsesMulti::CCM_One_ptr get_do_my_one (void);
  
    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::UsesMulti::CCM_Receiver_Context_var context_;
    Atomic_UShort nr_of_received_;
  };

  extern "C" USESMULTI_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_UsesMulti_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
