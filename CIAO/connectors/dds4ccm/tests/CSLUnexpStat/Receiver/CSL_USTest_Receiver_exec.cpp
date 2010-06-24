// -*- C++ -*-
//
// $Id$

//Test for on_unexpected_status with status LIVELINESS_CHANGED_STATUS' and 'SUBSCRIPTION_MATCHED_STATUS

#include "CSL_USTest_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_CSL_USTest_Receiver_Impl
{
  //============================================================
  // Pulser
  //============================================================
  Pulser::Pulser (Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  Pulser::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->callback_.read_all ();
    return 0;
  }


  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &subscription_matched_received,
                                                                  Atomic_Boolean &liveliness_changed_received,
                                                                  Atomic_ThreadId &thread_id_subcription_matched,
                                                                  Atomic_ThreadId &thread_id_liveliness_changed)
   : subscription_matched_received_ (subscription_matched_received),
     liveliness_changed_received_ (liveliness_changed_received),
     thread_id_subcription_matched_ (thread_id_subcription_matched),
     thread_id_liveliness_changed_ (thread_id_liveliness_changed)
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
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
  {
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
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind status_kind)
  {
    ACE_DEBUG ((LM_DEBUG, "RECEIVER\tConnectorStatusListener_exec_i::on_unexpected_status: "
                          "received <%C>\n",
                          CIAO::DDS4CCM::translate_statuskind (status_kind)));

    if (! ::CORBA::is_nil (the_entity) && status_kind == DDS::SUBSCRIPTION_MATCHED_STATUS)
      {
        this->subscription_matched_received_ = true;
        this->thread_id_subcription_matched_ = ACE_Thread::self ();
      }
    else if (! ::CORBA::is_nil(the_entity) && status_kind == DDS::LIVELINESS_CHANGED_STATUS)
      {
        this->liveliness_changed_received_ = true;
        this->thread_id_liveliness_changed_ = ACE_Thread::self ();
      }
  }

  //============================================================
  // TestTopic_RawListener_exec_i
  //============================================================
  TestTopic_RawListener_exec_i::TestTopic_RawListener_exec_i (Atomic_ULong &received,
                                                              Receiver_exec_i &callback)
      : received_ (received),
        callback_ (callback)
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
    ++this->received_;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_RawListener: ")
            ACE_TEXT ("received test_topic_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.x));
    if (this->received_ == 10)
      this->callback_.stop ();
  }

  void
  TestTopic_RawListener_exec_i::on_many_data (
    const TestTopicSeq & /*an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : subscription_matched_received_ (false),
      liveliness_changed_received_ (false),
      thread_id_listener_subscription_matched_ (0),
      thread_id_listener_liveliness_changed_ (0),
      received_(0)
  {
    this->lc_ = ::CCM_DDS::DataListenerControl::_nil ();
    this->pulser_= new Pulser (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_read_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CSL_US_Connector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_RawListener_exec_i (this->received_,
                                             *this);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->subscription_matched_received_,
                                               this->liveliness_changed_received_,
                                               this->thread_id_listener_subscription_matched_,
                                               this->thread_id_listener_liveliness_changed_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CSL_USTest::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::stop (void)
  {
    if (::CORBA::is_nil (this->lc_.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
      }
    this->lc_->mode (::CCM_DDS::NOT_ENABLED);
  }

  void
  Receiver_exec_i::start (void)
  {
    this->lc_ = this->context_->get_connection_info_out_data_control ();
    if (::CORBA::is_nil (this->lc_.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
      }
    this->lc_->mode (::CCM_DDS::NOT_ENABLED);
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::read_all ()
  {
    ::CSL_US_Connector::Reader_var reader =
      this->context_->get_connection_info_read_data ();

    try
      {
        TestTopicSeq readertest_info_seq;
        ::CCM_DDS::ReadInfoSeq readinfo_seq;
        if (this->received_ < 30)
          {
            reader->read_all (readertest_info_seq, readinfo_seq);
            ACE_DEBUG ((LM_DEBUG, "Read %u samples\n",
                                  readinfo_seq.length ()));
            this->received_ += readinfo_seq.length ();
          }
        else
          {
            this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->pulser_);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::start :Stop reading.\n")));
            delete this->pulser_;
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ALL: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->pulser_,
                0,
                ACE_Time_Value (6, 0),
                ACE_Time_Value (3, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer\n")));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
     if (!this->subscription_matched_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("RECEIVER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'SUBSCRIPTION_MATCHED_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("RECEIVER OK: Received the expected ")
                               ACE_TEXT ("'SUBSCRIPTION_MATCHED_STATUS'\n")
                    ));
      }
    if (!this->liveliness_changed_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("RECEIVER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'LIVELINESS_CHANGED_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("RECEIVER OK: Received the expected ")
                               ACE_TEXT ("'LIVELINESS_CHANGED_STATUS' \n")
                    ));
      }

    //test thread switch for SUBSCRIPTION_MATCHED_STATUS
    if (this->thread_id_listener_subscription_matched_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread ID for 'SUBSCRIPTION_MATCHED_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_subscription_matched_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: "
                              "Thread switch for 'SUBSCRIPTION_MATCHED_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_subscription_matched_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread switch for 'SUBSCRIPTION_MATCHED_STATUS' "
                              "doesn't seem to work! "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_subscription_matched_.value (),
                              ACE_Thread::self ()));
      }
    #else
    else if (ACE_OS::thr_equal (this->thread_id_listener_subscription_matched_.value (),
                                ACE_Thread::self ()))
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: 'SUBSCRIPTION_MATCHED_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_subscription_matched_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: 'SUBSCRIPTION_MATCHED_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_subscription_matched_.value (),
                              ACE_Thread::self ()));
      }
    #endif

    //test thread switch for LIVELINESS_CHANGED_STATUS
    if (this->thread_id_listener_liveliness_changed_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread ID for 'LIVELINESS_CHANGED_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_liveliness_changed_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: "
                              "Thread switch for 'LIVELINESS_CHANGED_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_liveliness_changed_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: "
                              "Thread switch for 'LIVELINESS_CHANGED_STATUS' "
                              "doesn't seem to work! "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_liveliness_changed_.value (),
                              ACE_Thread::self ()));
      }
    #else
    else if (ACE_OS::thr_equal (this->thread_id_listener_liveliness_changed_.value (),
                                ACE_Thread::self ()))
      {
        ACE_ERROR ((LM_ERROR, "RECEIVER ERROR: 'LIVELINESS_CHANGED_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_liveliness_changed_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "RECEIVER OK: 'LIVELINESS_CHANGED_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_liveliness_changed_.value (),
                              ACE_Thread::self ()));
      }
    #endif
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_USTest_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
