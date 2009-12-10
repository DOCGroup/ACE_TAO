// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Listen_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Reactor.h"
#include "ace/Task.h"

#include <map>

namespace CIAO_Listen_Test_Receiver_Impl
{
  class Receiver_exec_i;

  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;

  //============================================================
  // ListenTest_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ListenTest_Listener_exec_i
    : public virtual ::CCM_DDS::ListenTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ListenTest_Listener_exec_i (Atomic_ULong &);
    virtual ~ListenTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ListenTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ListenTest_Seq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_;
  };

  //============================================================
  // Invoker_exec_i
  //============================================================
  class Invoker_exec_i
    : public virtual ::CCM_ListenInvoker,
      public virtual ::CORBA::LocalObject
  {
  public:
    Invoker_exec_i (Receiver_exec_i & callback);
    virtual ~Invoker_exec_i (void);

    virtual void start_not_enabled ();
    virtual void start_listen_one_by_one (::CORBA::UShort keys,
                                      ::CORBA::ULong iteration);
    virtual void start_listen_many_by_many (::CORBA::UShort keys, 
                                      ::CORBA::ULong iterations);

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

    // Supported operations and attributes.
    // Component attributes.

    // Port operations.
    virtual ::CCM_DDS::ListenTest::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual ::CCM_ListenInvoker_ptr
    get_listen_invoke ();

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Listen_Test::CCM_Receiver_Context_var context_;
    //::CCM_DDS::ListenTest::Listen_var       listen_;
    Atomic_ULong received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Listen_Test_Receiver_Impl (void);
}

#endif /* ifndef */

