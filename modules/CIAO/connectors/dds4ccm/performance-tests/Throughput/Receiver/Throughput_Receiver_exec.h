// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Throughput_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"
#include "ace/Task.h"

namespace CIAO_Throughput_Receiver_Impl
{
  class Receiver_exec_i;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean> Atomic_Bool;

  //============================================================
  // ThroughputTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ThroughputTest_Listener_exec_i
    : public virtual ::Throughput::ThroughputTestConn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ThroughputTest_Listener_exec_i (Receiver_exec_i &callback);
    virtual ~ThroughputTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ThroughputTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ThroughputTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Receiver_exec_i &callback_;
  };

  //============================================================
  // ThroughputCommand_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ThroughputCommand_Listener_exec_i
    : public virtual ::Throughput::ThroughputCommandConn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ThroughputCommand_Listener_exec_i (Receiver_exec_i &callback);
    virtual ~ThroughputCommand_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ThroughputCommand & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ThroughputCommandSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Receiver_exec_i &callback_;
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

    void record_data (ThroughputTest & an_instance);
    void handle_run (ThroughputCommand & an_instance);

    virtual ::Throughput::ThroughputTestConn::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual ::Throughput::ThroughputCommandConn::CCM_Listener_ptr
    get_command_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_command_listen_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Throughput::CCM_Receiver_Context_var context_;

    CORBA::ULongLong count_;
    CORBA::ULongLong interval_messages_received_;
    CORBA::ULongLong interval_bytes_received_;
    CORBA::ULong interval_data_length_;
    CORBA::ULong run_;
    ACE_UINT64 first_time_;
    CORBA::ULongLong demand_;
    CORBA::ULongLong seq_num_;
    CORBA::ULongLong messages_lost_;
    Atomic_Bool log_;

    void start ();
    void reset_results (void);
    void show_results (const ACE_UINT64& interval_time);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Throughput_Receiver_Impl (void);
}

#endif /* ifndef */
