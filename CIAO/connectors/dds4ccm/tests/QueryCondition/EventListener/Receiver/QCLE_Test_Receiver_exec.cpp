// -*- C++ -*-
//
// $Id$

#include "QCLE_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION "2"
#define MAX_ITERATION "5"

#define SAMPLES_PER_KEY 2 //only iteration 3 and 4 should be coming in....

namespace CIAO_QCLE_Test_Receiver_Impl
{
  //============================================================
  // QueryConditionListenEventTest_Listener_exec_i
  //============================================================
  QueryConditionListenEventTest_Listener::QueryConditionListenEventTest_Listener (
                                              ACE_Thread_ID &thread_id,
                                              Atomic_Long &samples_received)
    : thread_id_ (thread_id),
      samples_received_ (samples_received)
  {
  }

  QueryConditionListenEventTest_Listener::~QueryConditionListenEventTest_Listener (void)
  {
  }

  void
  QueryConditionListenEventTest_Listener::on_one_data (
    const QueryConditionListenEventTest & an_instance,
    const ::CCM_DDS::ReadInfo & info)
  {
    ACE_Thread_ID t_id;
    this->thread_id_.handle (t_id.handle ());
    this->thread_id_.id (t_id.id ());
    ++this->samples_received_;

    ACE_DEBUG ((LM_DEBUG, "QueryConditionListenEventTest_Listener::on_one_data: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionListenEventTest_Listener::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionListenEventTest_Listener::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (an_instance.iteration <= ACE_OS::atoi (MIN_ITERATION) ||
        an_instance.iteration  > ACE_OS::atoi (MAX_ITERATION) )
      {
        ACE_ERROR ((LM_ERROR, "QueryConditionListenEventTest_Listener::on_one_data: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  QueryConditionListenEventTest_Listener::on_many_data (
                                  const QueryConditionListenEventTestSeq & /* an_instance */,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      samples_expected_ (keys_ * SAMPLES_PER_KEY),
      samples_received_ (0),
      thread_id_listener_ (0, 0)
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

  ::QCLE_Test::QueryConditionListenEventTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new QueryConditionListenEventTest_Listener (
                this->thread_id_listener_,
                this->samples_received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
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
      ::QCLE_Test::CCM_Receiver_Context::_narrow (ctx);
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
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate - "
                          "Set filter.\n"));

    CCM_DDS::QueryFilter filter;
    filter.expression = CORBA::string_dup (QUERY);
    filter.parameters.length (2);
    filter.parameters[0] = CORBA::string_dup (MIN_ITERATION);
    filter.parameters[1] = CORBA::string_dup (MAX_ITERATION);

    ::QCLE_Test::QueryConditionListenEventTestConnector::Reader_var reader =
      this->context_->get_connection_info_listen_data ();

    reader->query (filter);

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
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf [65];
    this->thread_id_listener_.to_string (list_buf);


    if (this->thread_id_listener_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "QF_EVENT: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_EVENT: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_EVENT: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                               "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
     }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_EVENT: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #endif
    if (this->samples_received_ != this->samples_expected_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_EVENT: ReaderListener: "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_EVENT: ReaderListener: "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_.value ()));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCLE_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
