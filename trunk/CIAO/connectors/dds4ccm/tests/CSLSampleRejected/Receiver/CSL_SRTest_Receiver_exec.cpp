// -*- C++ -*-
//
// $Id$

// Test for ConnectorStatusListener:REJECTED_SAMPLE status through  too much instances
// in Sender <max_instances>2</max_instances>, in Receiver <max_instances>1</max_instances>

#include "CSL_SRTest_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_CSL_SRTest_Receiver_Impl
{

//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &rejected,
                                                                  Atomic_ThreadId &thread_id)
    : rejected_ (rejected),
      thread_id_ (thread_id)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic (
    ::DDS::Topic_ptr /*the_topic*/,
    const DDS::InconsistentTopicStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos (
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected (
    ::DDS::DataReader_ptr the_reader,
    const DDS::SampleRejectedStatus & status)
  {
    this->thread_id_ = ACE_Thread::self ();
    if (status.last_reason == DDS::REJECTED_BY_INSTANCES_LIMIT &&
        ! ::CORBA::is_nil(the_reader))
      {
        this->rejected_ = true;
      }
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status (
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind /*status_kind*/)
  {
  }

  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    try
      {
        if (pulse_callback_.read_data ())
          {
            this->pulse_callback_.read_all();
          }
       }
    catch (...)
      {
      }
    return 0;
  }

  //============================================================
  // Facet Executor Implementation Class: TestTopic_Listener_exec_i
  //============================================================

  TestTopic_Listener_exec_i::TestTopic_Listener_exec_i (Atomic_ULong &received)
      : received_ (received)
  {
  }

  TestTopic_Listener_exec_i::~TestTopic_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::TestTopic_Listener
  void
  TestTopic_Listener_exec_i::on_many_data (
    const TestTopicSeq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  TestTopic_Listener_exec_i::on_one_data (
    const TestTopic & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_Listener: ")
            ACE_TEXT ("received testtopic_info for <%C> at %dl\n"),
            an_instance.key.in (),
            an_instance.x));
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : rate_ (0),
      read_data_ (true),
      raw_listen_ (false),
      received_ (0),
      rejected_(false),
      thread_id_listener_ (0)
  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.

  void
  Receiver_exec_i::read_all (void)
  {
    if (::CORBA::is_nil (this->reader_.in ()))
      {
        return;
      }
    TestTopicSeq_var TestTopic_infos;
    ::CCM_DDS::ReadInfoSeq_var readinfoseq;
    this->reader_->read_all(TestTopic_infos.out(), readinfoseq.out());
    for(CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
      {
        ACE_Time_Value tv;
        tv <<= readinfoseq[i].source_timestamp;
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                              ACE_TEXT ("-> UTC date =%#T\n"),
                              &tv));
      }
    for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
              ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
            i,
            TestTopic_infos[i].key.in (),
            TestTopic_infos[i].x));
      }
  }

  // Component attributes.
  ::CORBA::ULong
  Receiver_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Receiver_exec_i::rate (::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::Boolean
  Receiver_exec_i::read_data (void)
  {
    return this->read_data_;
  }

  void
  Receiver_exec_i::read_data (
    ::CORBA::Boolean read_data)
  {
    this->read_data_ = read_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::raw_listen (void)
  {
    return this->raw_listen_;
  }

  void
  Receiver_exec_i::raw_listen (
    ::CORBA::Boolean raw_listen)
  {
    this->raw_listen_ = raw_listen;
  }

  // Port operations.
  ::CSL_SRTest::TestTopicConn::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_Listener_exec_i (this->received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

   ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->rejected_,
                                               this->thread_id_listener_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CSL_SRTest::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    if (this->read_data ())
      {
       this->reader_ = this->context_->get_connection_info_out_data();
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (this->raw_listen_ ? ::CCM_DDS::ONE_BY_ONE : ::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(0, usec),
                                          ACE_Time_Value(0, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
     if (this->rejected_.value ())
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("RECEIVER OK: Have received the expected ")
                              ACE_TEXT ("'on_sample_rejected'\n")
                    ));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("RECEIVER ERROR: did not receive the expected ")
                              ACE_TEXT ("warning 'on_sample_rejected'\n")
                    ));
      }
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread ID for ConnectorStatusListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: "
                              "Thread switch for ConnectorStatusListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread switch for ConnectorStatusListener "
                              "doesn't seem to work! "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #else
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: ConnectorStatusListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: ConnectorStatusListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #endif
  }


  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_SRTest_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

