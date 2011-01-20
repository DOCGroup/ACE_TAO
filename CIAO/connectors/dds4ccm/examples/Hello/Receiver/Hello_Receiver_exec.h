// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1283

#ifndef CIAO_HELLO_RECEIVER_EXEC_0PTXJW_H_
#define CIAO_HELLO_RECEIVER_EXEC_0PTXJW_H_

#include /**/ "ace/pre.h"

#include "Hello_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Hello_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exh.cpp:53
  class HELLO_RECEIVER_EXEC_Export info_out_data_listener_exec_i
    : public virtual ::Hello::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    info_out_data_listener_exec_i (
      ::Hello::CCM_Receiver_Context_ptr ctx,
      Atomic_ULong &received,
      const ACE_CString &name);

    virtual ~info_out_data_listener_exec_i (void);

    // Operations and attributes from Hello::Listener

    virtual
    void on_one_data (
      const ::DDSHello & datum,
      const ::CCM_DDS::ReadInfo & info);

    virtual
    void on_many_data (
      const ::DDSHelloSeq & data,
      const ::CCM_DDS::ReadInfoSeq & infos);

  private:
    ::Hello::CCM_Receiver_Context_var ciao_context_;
    Atomic_ULong &received_;
    const ACE_CString &name_;
  };

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exh.cpp:53
  class HELLO_RECEIVER_EXEC_Export info_out_status_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    info_out_status_exec_i (
      ::Hello::CCM_Receiver_Context_ptr ctx,
      Atomic_ULong &lost);
    virtual ~info_out_status_exec_i (void);

    // Operations and attributes from CCM_DDS::PortStatusListener

    virtual
    void on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);

  private:
    ::Hello::CCM_Receiver_Context_var ciao_context_;
    Atomic_ULong &lost_;
  };

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exh.cpp:53
  class HELLO_RECEIVER_EXEC_Export info_out_connector_status_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    info_out_connector_status_exec_i (
      ::Hello::CCM_Receiver_Context_ptr ctx);
    virtual ~info_out_connector_status_exec_i (void);

    // Operations and attributes from CCM_DDS::ConnectorStatusListener

    virtual
    void on_inconsistent_topic (
      ::DDS::Topic_ptr the_topic,
      const ::DDS::InconsistentTopicStatus & status);

    virtual
    void on_requested_incompatible_qos (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedIncompatibleQosStatus & status);

    virtual
    void on_sample_rejected (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleRejectedStatus & status);

    virtual
    void on_offered_deadline_missed (
      ::DDS::DataWriter_ptr the_writer,
      const ::DDS::OfferedDeadlineMissedStatus & status);

    virtual
    void on_offered_incompatible_qos (
      ::DDS::DataWriter_ptr the_writer,
      const ::DDS::OfferedIncompatibleQosStatus & status);

    virtual
    void on_unexpected_status (
      ::DDS::Entity_ptr the_entity,
      ::DDS::StatusKind status_kind);

  private:
    ::Hello::CCM_Receiver_Context_var ciao_context_;
  };

  class HELLO_RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */


    virtual ::Hello::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual char *
    name (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    name (
      const char * name);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual ::CORBA::ULong
    expected_samples (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    expected_samples (
      ::CORBA::ULong expected_samples);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

  private:
    ::Hello::CCM_Receiver_Context_var ciao_context_;
    ::Hello::CCM_Listener_var ciao_info_out_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_info_out_status_;
    ::CCM_DDS::CCM_ConnectorStatusListener_var ciao_info_out_connector_status_;
    CORBA::ULong expected_;
    Atomic_ULong received_;
    Atomic_ULong lost_;
    ACE_CString name_;
  };

  extern "C" HELLO_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Receiver_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
