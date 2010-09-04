// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "CFTLS_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/OS_NS_Thread.h"

#include "ace/Task.h"

namespace CIAO_CFTLS_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> Atomic_Long;

  //============================================================
  // ContentFilteredTopicListenStateTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ContentFilteredTopicListenStateTest_Listener
    : public virtual ::CFTLS_Test::ContentFilteredTopicListenStateTestConnector::CCM_StateListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ContentFilteredTopicListenStateTest_Listener (
                            ACE_Thread_ID &thread_id,
                            Atomic_Long &samples_received);
    virtual ~ContentFilteredTopicListenStateTest_Listener (void);

    virtual void
    on_creation (const ContentFilteredTopicListenStateTest& an_instance,
                 const CCM_DDS::ReadInfo& read_info);

    virtual void
    on_one_update (const ContentFilteredTopicListenStateTest& an_instance,
                   const CCM_DDS::ReadInfo& read_info);

    virtual void
    on_many_updates (const ContentFilteredTopicListenStateTestSeq& an_instance,
                     const CCM_DDS::ReadInfoSeq& read_info);

    virtual void
    on_deletion (const ContentFilteredTopicListenStateTest& an_instance,
                 const CCM_DDS::ReadInfo& read_info);

  private:
    ACE_Thread_ID &thread_id_;
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

    virtual ::CFTLS_Test::ContentFilteredTopicListenStateTestConnector::CCM_StateListener_ptr
    get_info_state_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_state_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::CFTLS_Test::CCM_Receiver_Context_var context_;

    ACE_Thread_ID thread_id_listener_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    CORBA::Long samples_expected_;
    Atomic_Long samples_received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTLS_Test_Receiver_Impl (void);
}

#endif /* ifndef */
