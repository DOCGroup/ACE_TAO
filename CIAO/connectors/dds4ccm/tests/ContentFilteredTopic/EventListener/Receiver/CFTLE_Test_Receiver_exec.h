// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "CFTLE_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Reactor.h"
#include "ace/Task.h"



namespace CIAO_CFTLE_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> Atomic_Long;

  //============================================================
  // ContentFilteredTopicListenEventTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ContentFilteredTopicListenEventTest_Listener
    : public virtual ::CFTLE_Test::ContentFilteredTopicListenEventTestConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ContentFilteredTopicListenEventTest_Listener (
                            Atomic_ThreadId &thread_id,
                            Atomic_Long &samples_received);
    virtual ~ContentFilteredTopicListenEventTest_Listener (void);

    virtual void
    on_one_data (
      const ContentFilteredTopicListenEventTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ContentFilteredTopicListenEventTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ThreadId &thread_id_;
    Atomic_Long &samples_received_;
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

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::CFTLE_Test::ContentFilteredTopicListenEventTestConnector::CCM_Listener_ptr
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

  private:
    ::CFTLE_Test::CCM_Receiver_Context_var context_;

    Atomic_ThreadId thread_id_listener_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    CORBA::Long samples_expected_;
    Atomic_Long samples_received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTLE_Test_Receiver_Impl (void);
}

#endif /* ifndef */
