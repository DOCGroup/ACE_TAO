// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_

#include "LatencyTT_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_LatencyTT_Test_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long > Atomic_Long;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean> Atomic_Boolean;

  class Sender_exec_i;
  //============================================================
  // LatencyTTTest_Listener_exec_i
  //============================================================
  class SENDER_EXEC_Export LatencyTTTest_Listener_exec_i
    : public virtual ::CCM_DDS::LatencyTTTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    LatencyTTTest_Listener_exec_i (Sender_exec_i &callback);
    virtual ~LatencyTTTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const LatencyTTTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const LatencyTTTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Sender_exec_i &callback_;
  };

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

    virtual ::CCM_DDS::LatencyTTTest::CCM_Listener_ptr
    get_ping_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_ping_listen_status (void);

    void record_time (ACE_UINT64 receive_time);

    virtual ::CORBA::ULong iterations (void);
    virtual void iterations (::CORBA::ULong iterations);

    virtual ::CORBA::UShort sleep (void);
    virtual void sleep (::CORBA::UShort sleep);

    virtual ::CORBA::UShort number_of_sub (void);
    virtual void number_of_sub (::CORBA::UShort number_of_sub);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start (void);
    void stop (void);
    void write_one (void);
    void read (LatencyTTTest & instance,ACE_UINT64 receive_time);
    void calc_results (void);
    void reset_results (void);
    void init_values (void);
    void calculate_clock_overhead(void);


  private:
    ::LatencyTT_Test::CCM_Sender_Context_var context_;
    CCM_DDS::LatencyTTTest::Writer_var writer_;
    WriteTicker *ticker_;

    CORBA::UShort iterations_;
    CORBA::UShort datalen_;
    CORBA::UShort datalen_idx_;
    CORBA::UShort nr_of_runs_;
    CORBA::UShort sleep_;
    Atomic_Boolean matched_;
    CORBA::UShort number_of_subscribers_;
    Atomic_Long  tv_total_;
    Atomic_Long  tv_max_;
    Atomic_Long  tv_min_;
    CORBA::UShort count_;
    CORBA::UShort number_of_msg_;
    Atomic_Boolean timer_;
    Atomic_Boolean received_;
    Atomic_Long seq_num_;
    CORBA::Double sigma_duration_squared_;
    ACE_UINT64 start_time_;
    ACE_UINT64 start_time_test_;
    ACE_UINT64 end_time_test_;

    TAO_SYNCH_MUTEX mutex_;
    LatencyTTTest test_topic_;
    CORBA::Octet* buffer_;
    CORBA::Long* duration_times;
    CORBA::Short* datalen_range_;
    CORBA::Long _clock_overhead_;
};

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LatencyTT_Test_Sender_Impl (void);
}

#endif /* ifndef */
