// -*- C++ -*-
//
// $Id$

#include "QCTL_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY_1 "( (iteration > %0) AND (iteration < %1) )"
#define QUERY_2 "( (iteration < %0) OR (iteration > %1) )"

#define MIN_ITERATION "2"
#define MAX_ITERATION "5"

#define SAMPLES_PER_KEY_1 2

namespace CIAO_QCTL_Test_Receiver_Impl
{
  //============================================================
  // QueryConditionTwoListenersTest_Listener_1_exec_i
  //============================================================
  QueryConditionTwoListenersTest_Listener_1::QueryConditionTwoListenersTest_Listener_1 (
                                              ACE_Thread_ID &thread_id,
                                              Atomic_Long &samples_received)
    : thread_id_1_ (thread_id),
      samples_received_1_ (samples_received)
  {
  }

  QueryConditionTwoListenersTest_Listener_1::~QueryConditionTwoListenersTest_Listener_1 (void)
  {
  }

  void
  QueryConditionTwoListenersTest_Listener_1::on_one_data (
    const QueryConditionTwoListenersTest & an_instance,
    const ::CCM_DDS::ReadInfo & info)
  {
    ACE_Thread_ID t_id;
    this->thread_id_1_.handle (t_id.handle ());
    this->thread_id_1_.id (t_id.id ());
    ++this->samples_received_1_;

    ACE_DEBUG ((LM_DEBUG, "QueryConditionTwoListenersTest_Listener_1::on_one_data: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionTwoListenersTest_Listener_1::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionTwoListenersTest_Listener_1::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (an_instance.iteration <= ACE_OS::atoi (MIN_ITERATION) ||
        an_instance.iteration  > ACE_OS::atoi (MAX_ITERATION) )
      {
        ACE_ERROR ((LM_ERROR, "QueryConditionTwoListenersTest_Listener_1::on_one_data: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  QueryConditionTwoListenersTest_Listener_1::on_many_data (
                                  const QueryConditionTwoListenersTestSeq & /* an_instance */,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // QueryConditionTwoListenersTest_Listener_2_exec_i
  //============================================================
  QueryConditionTwoListenersTest_Listener_2::QueryConditionTwoListenersTest_Listener_2 (
                                              ACE_Thread_ID &thread_id,
                                              Atomic_Long &samples_received)
    : thread_id_2_ (thread_id),
      samples_received_2_ (samples_received)
  {
  }

  QueryConditionTwoListenersTest_Listener_2::~QueryConditionTwoListenersTest_Listener_2 (void)
  {
  }

  void
  QueryConditionTwoListenersTest_Listener_2::on_one_data (
    const QueryConditionTwoListenersTest & an_instance,
    const ::CCM_DDS::ReadInfo & info)
  {
    ACE_Thread_ID t_id;
    this->thread_id_2_.handle (t_id.handle ());
    this->thread_id_2_.id (t_id.id ());

    ++this->samples_received_2_;

    ACE_DEBUG ((LM_DEBUG, "QueryConditionTwoListenersTest_Listener_2::on_one_data: "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionTwoListenersTest_Listener_2::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QueryConditionTwoListenersTest_Listener_2::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.iteration));
      }
    if (an_instance.iteration > ACE_OS::atoi (MIN_ITERATION) &&
        an_instance.iteration  < ACE_OS::atoi (MAX_ITERATION) )
      {
        ACE_ERROR ((LM_ERROR, "QueryConditionTwoListenersTest_Listener_2::on_one_data: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  QueryConditionTwoListenersTest_Listener_2::on_many_data (
                                  const QueryConditionTwoListenersTestSeq & /* an_instance */,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      samples_expected_1_ (keys_ * SAMPLES_PER_KEY_1),
      samples_received_1_ (0),
      samples_expected_2_ (keys_ * (this->iterations_ - 4)),
      samples_received_2_ (0),
      thread_id_listener_1_ (0, 0),
      thread_id_listener_2_ (0, 0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc1 =
        this->context_->get_connection_info_listen_1_data_control ();
    dlc1->mode (::CCM_DDS::ONE_BY_ONE);

    ::CCM_DDS::DataListenerControl_var dlc2 =
        this->context_->get_connection_info_listen_2_data_control ();
    dlc2->mode (::CCM_DDS::ONE_BY_ONE);
  }

  ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_1_data_listener (void)
  {
    return new QueryConditionTwoListenersTest_Listener_1 (
                this->thread_id_listener_1_,
                this->samples_received_1_);
  }

  ::QCTL_Test::QueryConditionTwoListenersTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_2_data_listener (void)
  {
    return new QueryConditionTwoListenersTest_Listener_2 (
                this->thread_id_listener_2_,
                this->samples_received_2_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_1_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_2_status (void)
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
    // Iterations between min and max (including min and max) are not received.
    this->samples_expected_2_ = keys_ * (this->iterations_ - 4);
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
      ::QCTL_Test::CCM_Receiver_Context::_narrow (ctx);
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
                          "Set filter 1.\n"));

    CCM_DDS::QueryFilter filter_1;
    filter_1.expression = CORBA::string_dup (QUERY_1);
    filter_1.parameters.length (2);
    filter_1.parameters[0] = CORBA::string_dup (MIN_ITERATION);
    filter_1.parameters[1] = CORBA::string_dup (MAX_ITERATION);

    ::QCTL_Test::QueryConditionTwoListenersTestConnector::Reader_var reader_1 =
      this->context_->get_connection_info_listen_1_data ();

    reader_1->query (filter_1);

    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate - "
                      "Set filter 2.\n"));

    CCM_DDS::QueryFilter filter_2;
    filter_2.expression = CORBA::string_dup (QUERY_2);
    filter_2.parameters.length (2);
    filter_2.parameters[0] = CORBA::string_dup (MIN_ITERATION);
    filter_2.parameters[1] = CORBA::string_dup (MAX_ITERATION);

    ::QCTL_Test::QueryConditionTwoListenersTestConnector::Reader_var reader_2 =
      this->context_->get_connection_info_listen_2_data ();

    reader_2->query (filter_2);

    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate - "
                          "Start listening on both listeners.\n"));
    start ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
       //get current thread
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf_1 [65];
    this->thread_id_listener_1_.to_string (list_buf_1);

    if (this->thread_id_listener_1_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_1_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_1_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
     }
    #endif
   char list_buf_2 [65];
   this->thread_id_listener_2_.to_string (list_buf_2);
   if (this->thread_id_listener_2_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_2_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                               "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
     }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_2_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    #endif
    //first listener
    if (this->samples_received_1_ != this->samples_expected_1_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: ReaderListener: "
                              "Unexpected number of samples received "
                              "on first listener : "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_1_,
                              this->samples_received_1_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: ReaderListener: "
                              "Expected number of samples received "
                              "on first listener : "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_1_,
                              this->samples_received_1_.value ()));
      }
    //Second listener
    if (this->samples_received_2_ != this->samples_expected_2_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: QF_TWO_LISTENERS: ReaderListener: "
                              "Unexpected number of samples received` "
                              "on second listener : "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_2_,
                              this->samples_received_2_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "QF_TWO_LISTENERS: ReaderListener: "
                              "Expected number of samples received "
                              "on second listener : "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_2_,
                              this->samples_received_2_.value ()));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCTL_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
