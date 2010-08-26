// -*- C++ -*-
// $Id$

// Test for UPDATER

#include "Updater_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_Updater_Receiver_Impl
{
  //============================================================
  // ListenOneByOneTest_Listener_exec_i
  //============================================================
  ListenOneByOneTest_Listener_exec_i::ListenOneByOneTest_Listener_exec_i (
                                                      ACE_Thread_ID &thread_id)
    : thread_id_ (thread_id)
  {
  }

  ListenOneByOneTest_Listener_exec_i::~ListenOneByOneTest_Listener_exec_i (void)
  {
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_one_data (
                                  const TestTopic & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
  {
    ACE_Thread_ID t_id;
    this->thread_id_.handle (t_id.handle ());
    this->thread_id_.id (t_id.id ());

    ACE_DEBUG ((LM_DEBUG, "ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.x));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.x));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.x));
      }
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_many_data (
                                  const TestTopicSeq & ,
                                  const ::CCM_DDS::ReadInfoSeq & )
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::Updater::UpdaterConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new ListenOneByOneTest_Listener_exec_i (
                                  this->thread_id_listener_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Updater::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var dlc =
        this->context_->get_connection_info_out_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {

  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf [65];
    this->thread_id_listener_.to_string(list_buf);
    if (this->thread_id_listener_.id() == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "ONE_BY_ONE: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
  #else
      else if (this->thread_id_listener_ == ccm_thread_id)
        {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
     }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "ONE_BY_ONE: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #endif

  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Updater_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
