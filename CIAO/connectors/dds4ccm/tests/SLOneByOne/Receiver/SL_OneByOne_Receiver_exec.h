// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "SL_OneByOne_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Reactor.h"


namespace CIAO_SL_OneByOne_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> Atomic_Long;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;

  class Receiver_exec_i;

  //============================================================
  // StateListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export StateListener_exec_i
    : public virtual ::SL_OneByOne::SL_OneByOneConnector::CCM_StateListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    StateListener_exec_i (Atomic_Long &,
                          Atomic_Long &,
                          Atomic_Long &,
                          Atomic_Long &,
                          Atomic_ThreadId &);
    virtual ~StateListener_exec_i (void);

    virtual void
      on_creation (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
    virtual void
      on_one_update (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
     virtual void
      on_many_updates (
        const ::TestTopicSeq & data,
        const ::CCM_DDS::ReadInfoSeq & infos);
    virtual void
      on_deletion (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
  private:
     Atomic_Long &on_many_updates_;
     Atomic_Long &on_creation_;
     Atomic_Long &on_one_update_;
     Atomic_Long &on_deletion_;
     Atomic_ThreadId &thread_id_;
  };

  //============================================================
  // Receiver_exec_i
  //============================================================
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    virtual ::SL_OneByOne::SL_OneByOneConnector::CCM_StateListener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::SL_OneByOne::CCM_Receiver_Context_var context_;

    Atomic_Long on_many_updates_;
    Atomic_Long updater_data_;
    Atomic_Long on_creation_;
    Atomic_Long on_one_update_;
    Atomic_Long on_deletion_;
    Atomic_ThreadId thread_id_listener_;
  };
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_OneByOne_Receiver_Impl (void);
}

#endif /* ifndef */
