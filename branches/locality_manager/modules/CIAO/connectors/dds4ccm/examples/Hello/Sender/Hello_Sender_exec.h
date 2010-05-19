// -*- C++ -*-
// $Id$

#ifndef HELLO_SENDER_EXEC_H_
#define HELLO_SENDER_EXEC_H_


#include "Hello_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"

#include <map>

namespace CIAO_Hello_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

  class HELLO_SENDER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean &);
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
                               ::DDS::StatusKind  status_kind);
    private:
    Atomic_Boolean &ready_to_start_;

  };

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

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    // Supported operations and attributes.

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    void start (void);

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    void stop (void);

    void tick ();

    // Component attributes.
    virtual ::CORBA::ULong iterations (void);

    virtual void iterations (CORBA::ULong iterations);

    virtual char * message (void);

    virtual void message (const char *message);

    virtual ::CORBA::Boolean log_time (void);

    virtual void log_time (::CORBA::Boolean log_time);

    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);


    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_connector_status(void);

  private:
    ::Hello::CCM_Sender_Context_var context_;
    Hello::Writer_var writer_;
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

#endif /* ifndef */

