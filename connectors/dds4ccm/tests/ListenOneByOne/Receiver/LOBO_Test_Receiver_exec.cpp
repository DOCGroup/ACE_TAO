// -*- C++ -*-
//
// $Id$

#include "LOBO_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_LOBO_Test_Receiver_Impl
{
  //============================================================
  // ListenOneByOneTest_Listener_exec_i
  //============================================================
  ListenOneByOneTest_Listener_exec_i::ListenOneByOneTest_Listener_exec_i (
                                              Atomic_ULong &received_one_by_one,
                                              Atomic_ULong &received_many_by_many,
                                              Atomic_ThreadId &thread_id)
    : received_one_by_one_ (received_one_by_one),
      received_many_by_many_ (received_many_by_many),
      thread_id_ (thread_id)
  {
  }

  ListenOneByOneTest_Listener_exec_i::~ListenOneByOneTest_Listener_exec_i (void)
  {
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_one_data (
                                  const ListenOneByOneTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
  {
    this->thread_id_ = ACE_Thread::self ();
    ACE_DEBUG ((LM_DEBUG, "ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    ++this->received_one_by_one_;
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_many_data (
                                  const ListenOneByOneTestSeq & an_instance,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
    //just count. Is checked on ccm_remove.
    this->received_many_by_many_ += an_instance.length ();
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : received_one_by_one_ (0),
      received_many_by_many_ (0),
      thread_id_listener_ (0),
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
        this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  ::LOBO_Test::ListenOneByOneTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ListenOneByOneTest_Listener_exec_i (
                this->received_one_by_one_,
                this->received_many_by_many_,
                this->thread_id_listener_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_listen_connector_status (void)
  {
    return ::CCM_DDS::CCM_ConnectorStatusListener::_nil ();
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
      ::LOBO_Test::CCM_Receiver_Context::_narrow (ctx);
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
    start ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    CORBA::ULong expected = this->keys_ * this->iterations_;
    if (this->received_one_by_one_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: "
                               "Didn't receive samples on one_by_one "
                               "callback while mode is ONE_BY_ONE "
                               "expected <%u> - received <%u>\n",
                               expected,
                               this->received_one_by_one_.value ()));
      }
    else if (expected != this->received_one_by_one_.value ())
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: "
                               "Didn't receive the expected "
                               "number of samples on one_by_one "
                               "callback while mode is ONE_BY_ONE "
                               "expected <%u> - received <%u>\n",
                               expected,
                               this->received_one_by_one_.value ()));
      }
    if (this->received_many_by_many_.value () > 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: "
                               "Received samples on many_by_many "
                               "callback while mode is ONE_BY_ONE "
                               "expected <0> - received <%u>\n",
                               this->received_many_by_many_.value ()));
      }
    if (this->received_one_by_one_.value () > 0   &&
        this->received_many_by_many_.value () == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "ONE_BY_ONE: "
                               "Received only data on "
                               "one_by_one callback. "
                               "Test passed!\n"));
      }
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "ONE_BY_ONE: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: "
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
        ACE_ERROR ((LM_ERROR, "ERROR: ONE_BY_ONE: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "ONE_BY_ONE: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #endif
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LOBO_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
