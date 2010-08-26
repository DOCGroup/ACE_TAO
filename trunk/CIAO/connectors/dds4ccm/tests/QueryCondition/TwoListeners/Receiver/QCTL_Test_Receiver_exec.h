// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "QCTL_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"

namespace CIAO_QCTL_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> Atomic_Long;

  //============================================================
  // QueryConditionTwoListenersTest_Listener_1_exec_i
  //============================================================
  class RECEIVER_EXEC_Export QueryConditionTwoListenersTest_Listener_1
    : public virtual ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    QueryConditionTwoListenersTest_Listener_1 (
                            ACE_Thread_ID &thread_id,
                            Atomic_Long &samples_received);
    virtual ~QueryConditionTwoListenersTest_Listener_1 (void);

    virtual void
    on_one_data (
      const QueryConditionTwoListenersTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const QueryConditionTwoListenersTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    ACE_Thread_ID &thread_id_1_;
    Atomic_Long &samples_received_1_;
  };

  //============================================================
  // QueryConditionTwoListenersTest_Listener_2_exec_i
  //============================================================
  class RECEIVER_EXEC_Export QueryConditionTwoListenersTest_Listener_2
    : public virtual ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    QueryConditionTwoListenersTest_Listener_2 (
                            ACE_Thread_ID &thread_id,
                            Atomic_Long &samples_received);
    virtual ~QueryConditionTwoListenersTest_Listener_2 (void);

    virtual void
    on_one_data (
      const QueryConditionTwoListenersTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const QueryConditionTwoListenersTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    ACE_Thread_ID &thread_id_2_;
    Atomic_Long &samples_received_2_;
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

    virtual ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener_ptr
    get_info_listen_1_data_listener (void);

    virtual ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener_ptr
    get_info_listen_2_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_1_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_2_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::QCTL_Test::CCM_Receiver_Context_var context_;

    ACE_Thread_ID thread_id_listener_1_;
    ACE_Thread_ID thread_id_listener_2_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    CORBA::Long samples_expected_1_;
    Atomic_Long samples_received_1_;

    CORBA::Long samples_expected_2_;
    Atomic_Long samples_received_2_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCTL_Test_Receiver_Impl (void);
}

#endif /* ifndef */
