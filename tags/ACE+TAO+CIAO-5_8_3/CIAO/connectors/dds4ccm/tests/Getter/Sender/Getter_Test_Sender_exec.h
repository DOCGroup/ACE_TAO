// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "Getter_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Getter_Test_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // pulse_Generator
  //============================================================
  class pulse_Generator :
    public ACE_Event_Handler
  {
  public:
    pulse_Generator (Sender_exec_i &callback);

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Sender_exec_i &pulse_callback_;
  };

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class SENDER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Sender_exec_i &);
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
    void on_inconsistent_topic( ::DDS::Topic_ptr ,
                              const DDS::InconsistentTopicStatus & );
    virtual
    void on_requested_incompatible_qos( ::DDS::DataReader_ptr ,
                              const DDS::RequestedIncompatibleQosStatus & );
    virtual
    void on_sample_rejected( ::DDS::DataReader_ptr ,
                              const DDS::SampleRejectedStatus & );
    virtual
    void on_offered_deadline_missed( ::DDS::DataWriter_ptr ,
                              const DDS::OfferedDeadlineMissedStatus & );
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr ,
                              const DDS::OfferedIncompatibleQosStatus & );
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr ,
                              ::DDS::StatusKind );
  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // Sender_exec_i
  //============================================================
  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start (void);
    void tick (void);

  private:
    ::Getter_Test::CCM_Sender_Context_var context_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;
    bool          done_;
    bool          ccm_activated_;
    CORBA::ULong last_iter_;

    void start_timeout_tests (void);
    void write_many (void);

    pulse_Generator * ticker_;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Sender_Impl (void);
}

#endif /* ifndef */
