// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "QCLS_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Reactor.h"


namespace CIAO_QCLS_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;

  //============================================================
  // QueryConditionListenStateTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export QueryConditionListenStateTest_Listener
    : public virtual ::QCLS_Test::QueryConditionListenStateTestConnector::CCM_StateListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    QueryConditionListenStateTest_Listener (
                            Atomic_ThreadId &thread_id);
    virtual ~QueryConditionListenStateTest_Listener (void);

    virtual void
    on_creation (const QueryConditionListenStateTest& an_instance,
                 const CCM_DDS::ReadInfo& read_info);

    virtual void
    on_one_update (const QueryConditionListenStateTest& an_instance,
                   const CCM_DDS::ReadInfo& read_info);

    virtual void
    on_many_updates (const QueryConditionListenStateTestSeq& an_instance,
                     const CCM_DDS::ReadInfoSeq& read_info);

    virtual void
    on_deletion (const QueryConditionListenStateTest& an_instance,
                 const CCM_DDS::ReadInfo& read_info);

  private:
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

    void start ();

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::QCLS_Test::QueryConditionListenStateTestConnector::CCM_StateListener_ptr
    get_info_state_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_state_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::QCLS_Test::CCM_Receiver_Context_var context_;
    ::QCLS_Test::QueryConditionListenStateTestConnector::Reader_var reader_;

    Atomic_ThreadId thread_id_listener_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCLS_Test_Receiver_Impl (void);
}

#endif /* ifndef */
