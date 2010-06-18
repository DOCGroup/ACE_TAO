// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "PSL_DeadlineTest_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Reactor.h"


namespace CIAO_PSL_DeadlineTest_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;

  class Receiver_exec_i;

  //============================================================
  // TestTopic_RawListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export TestTopic_RawListener_exec_i
    : public virtual ::PSL_DeadlineTest::PSLDeadlineConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    TestTopic_RawListener_exec_i (void);
    virtual ~TestTopic_RawListener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopic & an_instance,
      const ::CCM_DDS::ReadInfo & info);

    virtual void
    on_many_data (
      const ::TestTopicSeq & data,
      const ::CCM_DDS::ReadInfoSeq & info );
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
  // PortStatusListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export PortStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PortStatusListener_exec_i (Atomic_Boolean &,
                               Atomic_Boolean &,
                               Atomic_ThreadId &,
                               Atomic_ThreadId &,
                               int );
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
    Atomic_Boolean &deadline_port_1_;
    Atomic_Boolean &deadline_port_2_;
    Atomic_ThreadId &thread_id_1_;
    Atomic_ThreadId &thread_id_2_;
    int port_nr_;
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

    // Port operations.
    virtual ::PSL_DeadlineTest::PSLDeadlineConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr get_info_out_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr get_info_get_status (void);

    // Operations from Components::SessionComponent.
    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    void read_all(void);

  private:
    ::PSL_DeadlineTest::CCM_Receiver_Context_var context_;
    read_action_Generator * ticker_;
    CORBA::ULong rate_;

    Atomic_Boolean deadline_port_1_;
    Atomic_Boolean deadline_port_2_;
    Atomic_ThreadId thread_id_listener_1_;
    Atomic_ThreadId thread_id_listener_2_;
};

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_DeadlineTest_Receiver_Impl (void);
}

#endif /* ifndef */

