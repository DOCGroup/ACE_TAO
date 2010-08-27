// -*- C++ -*-
//
// $Id$

// Test for SAMPLE_LOST status of the subscriber

#include "PSL_SampleLost_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_PSL_SampleLost_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
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
         this->pulse_callback_.read_all();
      }
    catch (...)
      {
        // @todo
      }
    return 0;
  }

  //============================================================
  // TestTopic_RawListener_exec_i
  //============================================================
  TestTopic_RawListener_exec_i::TestTopic_RawListener_exec_i (void)
  {
  }

  TestTopic_RawListener_exec_i::~TestTopic_RawListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::TestTopic_RawListener
  void
  TestTopic_RawListener_exec_i::on_one_data (
    const TestTopic & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_RawListener: ")
            ACE_TEXT ("received test_topic_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.x));
  }

  void
  TestTopic_RawListener_exec_i::on_many_data (
    const TestTopicSeq & /*an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  //============================================================
  // PortStatusListener_exec_i
  //============================================================
  PortStatusListener_exec_i::PortStatusListener_exec_i (Atomic_Boolean &sample_port_1,
                                                        Atomic_Boolean &sample_port_2,
                                                        ACE_Thread_ID &thread_id_1,
                                                        ACE_Thread_ID &thread_id_2,
                                                        int port_nr)
    : sample_port_1_(sample_port_1),
      sample_port_2_(sample_port_2),
      thread_id_1_ (thread_id_1),
      thread_id_2_ (thread_id_2),
      port_nr_(port_nr)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener
  void
    PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /*the_reader*/,
    const ::DDS::SampleLostStatus & status)
  {
    if (this->port_nr_ == 1)
      {
        ACE_Thread_ID t_id;
        this->thread_id_1_.handle (t_id.handle ());
        this->thread_id_1_.id (t_id.id ());
      }
    if (this->port_nr_ == 2)
      {
        ACE_Thread_ID t_id;
        this->thread_id_2_.handle (t_id.handle ());
        this->thread_id_2_.id (t_id.id ());
     }

    if (this->port_nr_ == 1 &&
        status.total_count > 0)
      {
        this->sample_port_1_ = true;
      }

    if (this->port_nr_ == 2 &&
        status.total_count > 0)
      {
        this->sample_port_2_ = true;
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10),
    sample_port_1_ (false),
    sample_port_2_ (false),
    thread_id_listener_1_ (0, 0),
    thread_id_listener_2_ (0, 0)
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
    ::PSL_SampleLost::PSL_SampleLostConnector::Reader_var reader =
      this->context_->get_connection_info_out_data();

    if (::CORBA::is_nil (reader.in ()))
      {
        return;
      }
    TestTopicSeq TestTopic_infos;
    ::CCM_DDS::ReadInfoSeq readinfoseq;
    try
      {
        reader->read_all(TestTopic_infos, readinfoseq);
        for(CORBA::ULong i = 0; i < readinfoseq.length(); ++i)
          {
            ACE_Time_Value tv;
            tv <<= readinfoseq[i].source_timestamp;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                                  ACE_TEXT ("-> UTC date =%#T\n"),
                                  &tv));
          }
        for(CORBA::ULong i = 0; i < TestTopic_infos.length(); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                  ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
                i,
                TestTopic_infos[i].key.in (),
                TestTopic_infos[i].x));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("internal error or no data\n")));
      }
  }

  // Component attributes.
  // Port operations.
  ::PSL_SampleLost::PSL_SampleLostConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_RawListener_exec_i (/*this->received_*/);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener get_info_out_status\n")));
    return new PortStatusListener_exec_i (this->sample_port_1_,
                                          this->sample_port_2_,
                                          this->thread_id_listener_1_,
                                          this->thread_id_listener_2_,
                                          1);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener get_info_get_status\n")));

    return new PortStatusListener_exec_i (this->sample_port_1_,
                                          this->sample_port_2_,
                                          this->thread_id_listener_1_,
                                          this->thread_id_listener_2_,
                                          2);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return ::CCM_DDS::CCM_ConnectorStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::PSL_SampleLost::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw ::CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(3, usec),
                                          ACE_Time_Value(3, usec)) == -1)
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
     if(!this->sample_port_1_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                         ACE_TEXT ("error 'on_sample_lost' on DDS_Listen port in Receiver\n")
              ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the expected ")
                               ACE_TEXT ("'on_sample_lost' in on DDS_Listen port Receiver\n")
                    ));
      }
    if (!this->sample_port_2_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                         ACE_TEXT ("error 'on_sample_lost' on DDS_Get port in Receiver\n")
              ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the expected ")
                               ACE_TEXT ("'on_sample_lost' in on DDS_GET port Receiver\n")
                    ));
      }
             //get current thread
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf_1 [65];
    this->thread_id_listener_1_.to_string (list_buf_1);

    //check thread switch for listener 1
    if (this->thread_id_listener_1_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for PortStatusListener I not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_1_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "OK : "
                              "Thread switch for PortStatusListener I seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                               "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
     }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread switch for PortStatusListener I "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_1_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: PortStatusListener I: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
     }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK : PortStatusListener I: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_1,
                              ccm_buf));
      }
    #endif

    //check thread switch for listener 2
    char list_buf_2 [65];
   this->thread_id_listener_2_.to_string (list_buf_2);
   if (this->thread_id_listener_2_.id () == 0)
     {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for PortStatusListener II not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_2_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "OK : "
                              "Thread switch for PortStatusListener II seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread switch for PortStatusListener II "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_2_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: PortStatusListener II: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK : PortStatusListener II: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_2,
                              ccm_buf));
      }
    #endif
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_SampleLost_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

