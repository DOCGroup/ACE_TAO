// -*- C++ -*-
//
// $Id$

#include "Listen_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Listen_Test_Receiver_Impl
{
  //============================================================
  // Invoker_exec_i
  //============================================================
  Invoker_exec_i::Invoker_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Invoker_exec_i::~Invoker_exec_i (void)
  {
  }

  void
  Invoker_exec_i::start_not_enabled ()
  {
  }

  void
  Invoker_exec_i::start_listen_one_by_one (::CORBA::UShort ,
                                     CORBA::ULong )
  {
  }

  void
  Invoker_exec_i::start_listen_many_by_many (::CORBA::UShort ,
                                    CORBA::ULong )
  {
  }

  //============================================================
  // ListenTest_Listener_exec_i
  //============================================================
  ListenTest_Listener_exec_i::ListenTest_Listener_exec_i (Atomic_ULong &received)
    : received_ (received)
  {
  }

  ListenTest_Listener_exec_i::~ListenTest_Listener_exec_i (void)
  {
  }

  void
  ListenTest_Listener_exec_i::on_one_data (
                                  const ListenTest & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
  }

  void
  ListenTest_Listener_exec_i::on_many_data (
                                  const ListenTest_Seq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : received_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::ListenTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ListenTest_Listener_exec_i (
                this->received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_ListenInvoker_ptr
  Receiver_exec_i::get_listen_invoke ()
  {
    return new Invoker_exec_i (*this);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Listen_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    //this->listen_ = this->context_->get_connection_info_get_fresh_data ();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Listen_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

