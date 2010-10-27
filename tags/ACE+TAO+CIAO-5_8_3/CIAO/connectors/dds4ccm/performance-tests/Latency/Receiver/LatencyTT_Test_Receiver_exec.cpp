// -*- C++ -*-
//
// $Id$

#include "LatencyTT_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_LatencyTT_Test_Receiver_Impl
{
  //============================================================
  // LatencyTTTest_Listener_exec_i
  //============================================================
  LatencyTTTest_Listener_exec_i::LatencyTTTest_Listener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  LatencyTTTest_Listener_exec_i::~LatencyTTTest_Listener_exec_i (void)
  {
  }

  void
  LatencyTTTest_Listener_exec_i::on_one_data (
                                  const LatencyTTTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->callback_.write_one(an_instance);
  }

  void
  LatencyTTTest_Listener_exec_i::on_many_data (
                                  const LatencyTTTestSeq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : count_ (0L)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::write_one (const LatencyTTTest & an_instance)
  {
    ++this->count_;
    this->writer_->write_one(an_instance, ::DDS::HANDLE_NIL);
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  ::LatencyTT_Test::LatencyTTTestConn::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new LatencyTTTest_Listener_exec_i (*this);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Receiver_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::LatencyTT_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    // Enable the datalistener.
    this->start();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
     this->writer_ = this->context_->get_connection_write_ping_data ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    if (this->count_ == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR RECEIVER: No messages sent back.\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SUMMARY RECEIVER:\n "
                              "Total number of messages sent back: %u \n",
                              this->count_));
      }
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LatencyTT_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
