// -*- C++ -*-
//
// $Id$

#include "CFTLS_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION "2"
#define MAX_ITERATION "5"

namespace CIAO_CFTLS_Test_Receiver_Impl
{
  //============================================================
  // ContentFilteredTopicListenStateTest_Listener_exec_i
  //============================================================
  ContentFilteredTopicListenStateTest_Listener::ContentFilteredTopicListenStateTest_Listener (
                                              Atomic_ThreadId &thread_id)
    : thread_id_ (thread_id)
  {
  }

  ContentFilteredTopicListenStateTest_Listener::~ContentFilteredTopicListenStateTest_Listener (void)
  {
  }

  void
  ContentFilteredTopicListenStateTest_Listener::on_creation (
                      const ContentFilteredTopicListenStateTest& an_instance,
                      const CCM_DDS::ReadInfo& /*read_info*/)
  {
    ACE_DEBUG ((LM_DEBUG, "ContentFilteredTopicListenStateTest_Listener::on_creation: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (an_instance.iteration <= ACE_OS::atoi (MIN_ITERATION) ||
        an_instance.iteration  > ACE_OS::atoi (MAX_ITERATION) )
      {
        ACE_ERROR ((LM_ERROR, "ContentFilteredTopicListenStateTest_Listener::on_creation: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  ContentFilteredTopicListenStateTest_Listener::on_one_update (
                      const ContentFilteredTopicListenStateTest& an_instance,
                      const CCM_DDS::ReadInfo& /*read_info*/)
  {
    this->thread_id_ = ACE_Thread::self ();
    ACE_DEBUG ((LM_DEBUG, "ContentFilteredTopicListenStateTest_Listener::on_one_update: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (an_instance.iteration <= ACE_OS::atoi (MIN_ITERATION) ||
        an_instance.iteration  > ACE_OS::atoi (MAX_ITERATION) )
      {
        ACE_ERROR ((LM_ERROR, "ContentFilteredTopicListenStateTest_Listener::on_one_update: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  ContentFilteredTopicListenStateTest_Listener::on_many_updates (
                      const ContentFilteredTopicListenStateTestSeq& /*an_instance*/,
                      const CCM_DDS::ReadInfoSeq& /*read_info*/)
  {
  }

  void
  ContentFilteredTopicListenStateTest_Listener::on_deletion (
                      const ContentFilteredTopicListenStateTest& /*an_instance*/,
                      const CCM_DDS::ReadInfo& /*read_info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : thread_id_listener_ (0),
      iterations_ (10),
      keys_ (5)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc =
        this->context_->get_connection_info_state_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  ::CFTLS_Test::ContentFilteredTopicListenStateTestConnector::CCM_StateListener_ptr
  Receiver_exec_i::get_info_state_data_listener (void)
  {
    return new ContentFilteredTopicListenStateTest_Listener (
                this->thread_id_listener_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_state_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CFTLS_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->reader_ = this->context_->get_connection_info_state_data ();
    if (CORBA::is_nil (this->reader_.in ()))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::configuration_complete - "
                              "Error get reader connection.\n"));
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate - "
                          "Start listening.\n"));
    start ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "QF_STATE: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_STATE: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #else
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_STATE: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_STATE: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #endif
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTLS_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
