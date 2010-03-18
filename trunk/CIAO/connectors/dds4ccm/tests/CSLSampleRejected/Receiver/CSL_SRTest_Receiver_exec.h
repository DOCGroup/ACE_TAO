// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "CSL_SRTest_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Reactor.h"
#include "ace/Task.h"

namespace CIAO_CSL_SRTest_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> Atomic_ULong;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean> Atomic_Boolean;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;

  class Receiver_exec_i;

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean  &,
                                    Atomic_ThreadId &);
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
    void on_offered_incompatible_qos (::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
      void on_unexpected_status (::DDS::Entity_ptr the_entity,
                                 ::DDS::StatusKind status_kind);
  private:
    Atomic_Boolean &rejected_;
    Atomic_ThreadId &thread_id_;
  };

  //============================================================
  // read_action_Generator
  //============================================================
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback);

    ~read_action_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

  private:
    Receiver_exec_i &pulse_callback_;
  };

  //============================================================
  // TestTopic_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export TestTopic_Listener_exec_i
    : public virtual ::CSL_SRTest::TestTopicConn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    TestTopic_Listener_exec_i (Atomic_ULong &);
    virtual ~TestTopic_Listener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopic & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const TestTopicSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_;
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
    void read_all (void);

    // Component attributes.
    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    virtual ::CORBA::Boolean read_data (void);

    virtual void read_data (::CORBA::Boolean read_data);

    virtual ::CORBA::Boolean raw_listen (void);

    virtual void raw_listen (::CORBA::Boolean raw_listen);

    // Port operations.
    virtual ::CSL_SRTest::TestTopicConn::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);


    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::CSL_SRTest::CCM_Receiver_Context_var context_;
    ::CSL_SRTest::TestTopicConn::Reader_var reader_;

    read_action_Generator * ticker_;
    CORBA::ULong rate_;
    CORBA::Boolean read_data_, raw_listen_;
    Atomic_ULong received_;
    Atomic_Boolean rejected_;
    Atomic_ThreadId thread_id_listener_;
  };
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_SRTest_Receiver_Impl (void);
}

#endif /* ifndef */

