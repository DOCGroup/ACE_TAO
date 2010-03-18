// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_

#include "Throughput_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Throughput_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

class SENDER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean &, int,  Sender_exec_i &callback_);
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

  private:
    Sender_exec_i &callback_;
    Atomic_Boolean &matched_;
    int number_of_subscribers_; 
  
  };
  //============================================================
  // WriteTicker
  //============================================================
  class WriteTicker :
    public ACE_Event_Handler
  {
  public:
    WriteTicker (Sender_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    /// Maintains a handle that actually process the event
    Sender_exec_i &callback_;
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_connector_status (void);

    virtual ::CORBA::ULong start_load (void);
    virtual void start_load (::CORBA::ULong start_load);
    
    virtual ::CORBA::ULongLong max_load (void);
    virtual void max_load (::CORBA::ULongLong max_load);

    virtual ::CORBA::ULong incr_load (void);
    virtual void incr_load (::CORBA::ULong incr_load);
 
    virtual ::CORBA::UShort recover_time (void);
    virtual void recover_time (::CORBA::UShort recover_time);

    virtual ::CORBA::UShort number_of_sub (void);
    virtual void number_of_sub (::CORBA::UShort number_of_sub);

    virtual ::CORBA::UShort datalen (void);
    virtual void datalen (::CORBA::UShort datalen);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start(void);
    void stop (void);
    void write(void);

  private:
    ::Throughput::CCM_Sender_Context_var context_;
    CCM_DDS::ThroughputTest::Writer_var writer_;
    CCM_DDS::ThroughputCommand::Writer_var cmd_writer_;
 
    WriteTicker *ticker_;

    CORBA::ULong start_load_;
    CORBA::ULongLong max_load_;
    CORBA::ULong incr_load_;
    CORBA::UShort datalen_;
    CORBA::UShort recover_time_;
    CORBA::UShort duration_run_;
    Atomic_Boolean matched_;
    CORBA::UShort number_of_subscribers_;
    CORBA::ULongLong number_of_msg_;
    Atomic_Boolean timer_;
    CORBA::ULongLong load_;
    CORBA::UShort overhead_size_;
    
    CORBA::UShort count_;
    ACE_UINT64 start_time_;

    ThroughputTest test_topic_;
    ThroughputCommand test_topic_cmd_;
    CORBA::Octet* buffer_;
};

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Throughput_Sender_Impl (void);
}

#endif /* ifndef */
