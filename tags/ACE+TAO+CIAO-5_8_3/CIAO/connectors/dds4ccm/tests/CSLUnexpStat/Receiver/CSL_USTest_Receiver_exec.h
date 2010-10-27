// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "CSL_USTest_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_CSL_USTest_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Receiver_exec_i;

  //============================================================
  // Pulser
  //============================================================
  class Pulser :
    public ACE_Event_Handler
  {
  public:
    Pulser (Receiver_exec_i &callback);

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Receiver_exec_i &callback_;
  };

  //============================================================
  // TestTopic_RawListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export TestTopic_RawListener_exec_i
    : public virtual ::CSL_US_Connector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    TestTopic_RawListener_exec_i (Atomic_ULong &,
                                  Receiver_exec_i &callback);
    virtual ~TestTopic_RawListener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopic & an_instance,
      const ::CCM_DDS::ReadInfo & info);

    virtual void
    on_many_data (
      const ::TestTopicSeq & data,
      const ::CCM_DDS::ReadInfoSeq & info);

  private:
    Atomic_ULong &received_;
    Receiver_exec_i &callback_;
  };

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean &,
                                    Atomic_Boolean &,
                                    ACE_Thread_ID &,
                                    ACE_Thread_ID &);

    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
    void on_inconsistent_topic (::DDS::Topic_ptr the_topic,
                                const DDS::InconsistentTopicStatus & status);
    virtual
    void on_requested_incompatible_qos (::DDS::DataReader_ptr the_reader,
                                        const DDS::RequestedIncompatibleQosStatus & status);
    virtual
    void on_sample_rejected (::DDS::DataReader_ptr the_reader,
                             const DDS::SampleRejectedStatus & status);
    virtual
    void on_offered_deadline_missed (::DDS::DataWriter_ptr the_writer,
                                     const DDS::OfferedDeadlineMissedStatus & status);
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr the_entity,
                               ::DDS::StatusKind  status_kind);

  private:
    Atomic_Boolean &subscription_matched_received_;
    Atomic_Boolean &liveliness_changed_received_;
    ACE_Thread_ID &thread_id_subcription_matched_;
    ACE_Thread_ID &thread_id_liveliness_changed_;
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

    // Supported operations and attributes.
    void stop (void);
    void start (void);
    void read_all (void);

    // Port operations.
    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CSL_US_Connector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_read_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::CSL_USTest::CCM_Receiver_Context_var context_;
    ::CCM_DDS::DataListenerControl_var lc_;

    Pulser *pulser_;

    Atomic_Boolean subscription_matched_received_;
    Atomic_Boolean liveliness_changed_received_;
    ACE_Thread_ID thread_id_listener_subscription_matched_;
    ACE_Thread_ID thread_id_listener_liveliness_changed_;
    Atomic_ULong received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_USTest_Receiver_Impl (void);
}

#endif /* ifndef */
