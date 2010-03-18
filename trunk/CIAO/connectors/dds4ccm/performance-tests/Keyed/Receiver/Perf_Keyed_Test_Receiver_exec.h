// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Perf_Keyed_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"
#include "ace/Task.h"

#include <map>

namespace CIAO_Perf_Keyed_Test_Receiver_Impl
{
  class Receiver_exec_i;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> Atomic_ULong;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> Atomic_Bool;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> Atomic_Long;


  //============================================================
  // PerfKeyedTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export PerfKeyedTest_Listener_exec_i
    : public virtual ::CCM_DDS::PerfKeyedTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PerfKeyedTest_Listener_exec_i (Receiver_exec_i &callback);
    virtual ~PerfKeyedTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const PerfKeyedTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const PerfKeyedTestSeq & an_instance,
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

    void start ();
    void record_time (unsigned long datalen);

    virtual ::CCM_DDS::PerfKeyedTest::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void write_one (PerfKeyedTest & an_instance);

  private:
    ::Perf_Keyed_Test::CCM_Receiver_Context_var context_;
    CCM_DDS::PerfKeyedTest::Writer_var writer_;

    Atomic_ULong  count_;
    ACE_UINT64 interval_time_;
    Atomic_Long interval_messages_received_;
    Atomic_Long messages_received_;
    Atomic_Long interval_bytes_received_;
    Atomic_Long bytes_received_;
    Atomic_Long interval_data_length_;
    ACE_UINT64 first_time_;
    Atomic_Bool   finished_;

  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Receiver_Impl (void);
}

#endif /* ifndef */
