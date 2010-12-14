// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Updater_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


namespace CIAO_Updater_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Receiver_exec_i;

  //============================================================
  // ListenOneByOneTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ListenOneByOneTest_Listener_exec_i
    : public virtual ::Updater::UpdaterConnector::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenOneByOneTest_Listener_exec_i (ACE_Thread_ID &);

    virtual ~ListenOneByOneTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const TestTopic& an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const TestTopicSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    ACE_Thread_ID &thread_id_;
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
    virtual ::Updater::UpdaterConnector::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Updater::CCM_Receiver_Context_var context_;
    ACE_Thread_ID thread_id_listener_;
};

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Updater_Receiver_Impl (void);
}

#endif /* ifndef */
