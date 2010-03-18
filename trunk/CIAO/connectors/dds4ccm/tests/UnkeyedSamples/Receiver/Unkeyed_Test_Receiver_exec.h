// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Unkeyed_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_Unkeyed_Test_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;

  class RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i ();
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual void
    on_inconsistent_topic( ::DDS::Topic_ptr the_topic,
                           const DDS::InconsistentTopicStatus & status);
    virtual void
    on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                           const DDS::RequestedIncompatibleQosStatus & status);
    virtual void
    on_sample_rejected( ::DDS::DataReader_ptr the_reader,
                           const DDS::SampleRejectedStatus & status);
    virtual void
    on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                           const DDS::OfferedDeadlineMissedStatus & status);
    virtual void
    on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer,
                           const DDS::OfferedIncompatibleQosStatus & status);
    virtual void on_unexpected_status( ::DDS::Entity_ptr the_entity,
                           ::DDS::StatusKind  status_kind);
  };

  class RECEIVER_EXEC_Export UnkeyedTest_Listener_exec_i
    : public virtual ::CCM_DDS::UnkeyedTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    UnkeyedTest_Listener_exec_i (Atomic_ULong &);
    virtual ~UnkeyedTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const UnkeyedTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const UnkeyedTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_;
  };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Component attributes.
    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    // Port operations.
    virtual ::CCM_DDS::UnkeyedTest::CCM_Listener_ptr
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
    ::Unkeyed_Test::CCM_Receiver_Context_var context_;
    ::CCM_DDS::UnkeyedTest::Reader_var reader_;
    ::CCM_DDS::UnkeyedTest::Getter_var getter_;

    CORBA::UShort iterations_;
    Atomic_ULong received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Receiver_Impl (void);
}

#endif /* ifndef */

