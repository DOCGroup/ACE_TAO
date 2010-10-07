// -*- C++ -*-
// $Id$

#ifndef HELLO_RECEIVER_EXEC_H_
#define HELLO_RECEIVER_EXEC_H_


#include "Hello_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Hello_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;

  class HELLO_RECEIVER_EXEC_Export DDSHello_Listener_exec_i
    : public virtual ::Hello::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    DDSHello_Listener_exec_i (Atomic_ULong &,
                              const ACE_CString &);
    virtual ~DDSHello_Listener_exec_i (void);

    virtual void
    on_one_data (
      const DDSHello & an_instance,
      const ::CCM_DDS::ReadInfo & info);

    virtual void
    on_many_data (
      const DDSHelloSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_;
    const ACE_CString &name_;
  };

  class HELLO_RECEIVER_EXEC_Export PortStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PortStatusListener_exec_i (Atomic_ULong &);
    virtual ~PortStatusListener_exec_i (void);

    virtual void
    on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual void
    on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);
  private:
    Atomic_ULong &lost_;
  };

  class HELLO_RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (void);
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
    void on_inconsistent_topic( ::DDS::Topic_ptr the_topic,
                                    const DDS::InconsistentTopicStatus & status);
    virtual
    void on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                                    const DDS::RequestedIncompatibleQosStatus & status);
    virtual
    void on_sample_rejected( ::DDS::DataReader_ptr the_reader,
                                    const DDS::SampleRejectedStatus & status);
    virtual
      void on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                                    const DDS::OfferedDeadlineMissedStatus & status);
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer,
                                    const DDS::OfferedIncompatibleQosStatus & status);
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr the_entity,
                                    ::DDS::StatusKind  status_kind);
    virtual
    void on_subscription_matched (::DDS::DataReader_ptr the_reader,
                                    const ::DDS::SubscriptionMatchedStatus & status);

  };

  class HELLO_RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Supported operations and attributes.
    // Port operations.

    virtual ::CORBA::ULong expected_samples (void);
    virtual void expected_samples (::CORBA::ULong expected_samples);

    virtual char * name (void);
    virtual void name (const char *name);

    virtual ::Hello::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    // Operations from Components::SessionComponent.

    virtual void
    set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello::CCM_Receiver_Context_var context_;
    CORBA::ULong expected_;
    Atomic_ULong received_;
    Atomic_ULong lost_;
    ACE_CString name_;
  };

  extern "C" HELLO_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Receiver_Impl (void);
}

#endif /* ifndef */

