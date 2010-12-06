// -*- C++ -*-
// $Id$

#ifndef CIAO_HELLO_SENDER_EXEC_YPB9XX_H_
#define CIAO_HELLO_SENDER_EXEC_YPB9XX_H_

#include /**/ "ace/pre.h"

#include "Hello_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"
#include <map>

namespace CIAO_Hello_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exh.cpp:53
  class HELLO_SENDER_EXEC_Export connector_status_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    connector_status_exec_i (
      ::Hello::CCM_Sender_Context_ptr ctx,
      Atomic_Boolean &ready_to_start);
    virtual ~connector_status_exec_i (void);

    // Operations and attributes from ::CCM_DDS::ConnectorStatusListener

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
    ::Hello::CCM_Sender_Context_var ciao_context_;
    Atomic_Boolean &ready_to_start_;
  };

  class Sender_exec_i;

  class pulse_Generator :
    public ACE_Event_Handler
  {
  public:
    pulse_Generator (Sender_exec_i &callback);

    ~pulse_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Sender_exec_i &pulse_callback_;
  };


  class HELLO_SENDER_EXEC_Export Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    //@{
    /** Supported operations and attributes. */
    void start (void);

    void stop (void);

    void tick ();

    //@}

    //@{
    /** Component attributes and port operations. */


    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual ::CORBA::ULong
    rate (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    rate (
      ::CORBA::ULong rate);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual ::CORBA::ULong
    iterations (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    iterations (
      ::CORBA::ULong iterations);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual char *
    message (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    message (
      const char * message);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual ::CORBA::Boolean
    log_time (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:36

    virtual void
    log_time (
      ::CORBA::Boolean log_time);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_connector_status (void);
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
    ::Hello::CCM_Sender_Context_var ciao_context_;
    ::CCM_DDS::CCM_ConnectorStatusListener_var ciao_connector_status_;
    pulse_Generator * ticker_;
    CORBA::ULong rate_;
    CORBA::ULong iteration_, iterations_;
    CORBA::Boolean log_time_;
    ACE_CString msg_;

    ACE_CString create_message (
          const ACE_CString &msg);
    Atomic_Boolean ready_to_start_;
  };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
