// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "LNE_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_LNE_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> Atomic_ULong;

  //============================================================
  // ListenNotEnabledTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ListenNotEnabledTest_Listener_exec_i
    : public virtual ::CCM_DDS::ListenNotEnabledTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenNotEnabledTest_Listener_exec_i (
                            Atomic_ULong &received_one_by_one,
                            Atomic_ULong &received_many_by_many);
    virtual ~ListenNotEnabledTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ListenNotEnabledTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ListenNotEnabledTestSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_one_by_one_;
    Atomic_ULong &received_many_by_many_;
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

    virtual ::CCM_DDS::ListenNotEnabledTest::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_listen_connector_status (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::LNE_Test::CCM_Receiver_Context_var context_;

    Atomic_ULong received_one_by_one_;
    Atomic_ULong received_many_by_many_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LNE_Test_Receiver_Impl (void);
}

#endif /* ifndef */
