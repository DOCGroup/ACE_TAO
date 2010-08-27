// -*- C++ -*-
// $Id$


//Test for on_unexpected_status with status LIVELINESS_CHANGED_STATUS' and 'PUBLICATION_MATCHED_STATUS
#include "CSL_USTest_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_CSL_USTest_Sender_Impl
{
  //============================================================
  // pulse_Generator
  //============================================================
  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &publication_matched_received,
                                                                  Atomic_Boolean &liveliness_lost_received,
                                                                  Atomic_Boolean &reliable_dr_activity_changed_received,
                                                                  Atomic_Boolean &reliable_writer_cache_changed_received,
                                                                  ACE_Thread_ID &thread_id_publication_matched,
                                                                  ACE_Thread_ID &thread_id_liveliness_lost,
                                                                  ACE_Thread_ID &thread_id_reliable_dr_activity_changed,
                                                                  ACE_Thread_ID &thread_id_reliable_writer_cache_changed)
   : publication_matched_received_ (publication_matched_received),
     liveliness_lost_received_ (liveliness_lost_received),
     reliable_dr_activity_changed_received_ (reliable_dr_activity_changed_received),
     reliable_writer_cache_changed_received_ (reliable_writer_cache_changed_received),
     thread_id_publication_matched_ (thread_id_publication_matched),
     thread_id_liveliness_lost_ (thread_id_liveliness_lost),
     thread_id_reliable_dr_activity_changed_ (thread_id_reliable_dr_activity_changed),
     thread_id_reliable_writer_cache_changed_ (thread_id_reliable_writer_cache_changed)
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
    ACE_Thread_ID t_id;
    ACE_DEBUG ((LM_DEBUG, "SENDER\t\tConnectorStatusListener_exec_i::on_unexpected_status: "
                          "received <%C>\n",
                          CIAO::DDS4CCM::translate_statuskind (status_kind)));

    if (! ::CORBA::is_nil(the_entity) && status_kind == DDS::LIVELINESS_LOST_STATUS)
      {
        this->liveliness_lost_received_ = true;
        this->thread_id_liveliness_lost_ = t_id;
    }
    else if (! ::CORBA::is_nil(the_entity) && status_kind == DDS::PUBLICATION_MATCHED_STATUS)
      {
        this->publication_matched_received_ = true;
        this->thread_id_publication_matched_ = t_id;

      }
    else if (! ::CORBA::is_nil(the_entity) && status_kind == DDS::RELIABLE_READER_ACTIVITY_CHANGED_STATUS)
      {
        this->reliable_dr_activity_changed_received_ = true;
        this->thread_id_reliable_dr_activity_changed_ = t_id;
      }
    else if (! ::CORBA::is_nil(the_entity) && status_kind == DDS::RELIABLE_WRITER_CACHE_CHANGED_STATUS)
      {
        this->reliable_writer_cache_changed_received_ = true;
        this->thread_id_reliable_writer_cache_changed_ = t_id;
      }
  }

  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : publication_matched_received_ (false),
      liveliness_lost_received_ (false),
      reliable_dr_activity_changed_received_ (false),
      reliable_writer_cache_changed_received_ (false),
      thread_id_listener_publication_matched_ (0, 0),
      thread_id_listener_liveliness_lost_ (0, 0),
      thread_id_reliable_dr_activity_changed_ (0, 0),
      thread_id_reliable_writer_cache_changed_ (0, 0)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->publication_matched_received_,
                                               this->liveliness_lost_received_,
                                               this->reliable_dr_activity_changed_received_,
                                               this->reliable_writer_cache_changed_received_,
                                               this->thread_id_listener_publication_matched_,
                                               this->thread_id_listener_liveliness_lost_,
                                               this->thread_id_reliable_dr_activity_changed_,
                                               this->thread_id_reliable_writer_cache_changed_);
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::tick ()
  {
    ::CSL_US_Connector::Writer_var writer =
      this->context_->get_connection_test_topic_write_data ();

    for (CSL_SRTest_Table::iterator i = this->_ktests_.begin ();
         i != this->_ktests_.end ();
         ++i)
      {
        try
          {
            if (! ::CORBA::is_nil (writer.in ()))
              {
                writer->write_one(i->second,::DDS::HANDLE_NIL);
                ACE_DEBUG ((LM_DEBUG, "Written sample: <%C> - <%d>\n",
                  i->first.c_str (),
                  i->second->x));
                i->second->x++;
              }
            else ACE_ERROR ((LM_ERROR, "Unable to write sample: <%C> - <%d>\n",
                                        i->first.c_str (),
                                        i->second->x));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating topic for <%C>.\n"),
                    i->first.c_str ()));
          }
     }
  }

  void
  Sender_exec_i::add_instance_of_topic (const char * key, int x)
  {
    TestTopic *new_key = new TestTopic;
    new_key->key = CORBA::string_dup(key);
    new_key->x = x;
    this->_ktests_[key] = new_key;
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CSL_USTest::CCM_Sender_Context::_narrow (ctx);

    if (::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    //add 2 different instances of topic
    this->add_instance_of_topic ("ONE", 1);
    this->add_instance_of_topic ("TWO", 2);
    // calculate the interval time
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (3, 0),
                ACE_Time_Value (1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer\n")));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (!this->publication_matched_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'PUBLICATION_MATCHED_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                               ACE_TEXT ("'PUBLICATION_MATCHED_STATUS'\n")
                    ));
      }
    if (!this->liveliness_lost_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'LIVELINESS_LOST_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                               ACE_TEXT ("'LIVELINESS_LOST_STATUS'\n")
                    ));
      }
    if (!this->reliable_dr_activity_changed_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'RELIABLE_READER_ACTIVITY_CHANGED_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                               ACE_TEXT ("'RELIABLE_READER_ACTIVITY_CHANGED_STATUS'\n")
                    ));
      }
    if (!this->reliable_writer_cache_changed_received_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: Didn't receive the expected ")
                              ACE_TEXT ("'RELIABLE_WRITER_CACHE_CHANGED_STATUS'\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                               ACE_TEXT ("'RELIABLE_WRITER_CACHE_CHANGED_STATUS'\n")
                    ));
      }
    //get current thread
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    //test thread switch for PUBLICATION_MATCHED_STATUS

    char list_buf_pm [65];
    this->thread_id_listener_publication_matched_.to_string(list_buf_pm);
    if (this->thread_id_listener_publication_matched_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for 'PUBLICATION_MATCHED_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_publication_matched_== ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: "
                              "Thread switch for 'PUBLICATION_MATCHED_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf_pm,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread switch for 'PUBLICATION_MATCHED_STATUS' "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_pm,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_publication_matched_== ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: 'PUBLICATION_MATCHED_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_pm,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: 'PUBLICATION_MATCHED_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_pm,
                              ccm_buf));
      }
    #endif

    //test thread switch for LIVELINESS_LOST_STATUS
     char list_buf_ll [65];
    this->thread_id_listener_liveliness_lost_.to_string(list_buf_ll);
    if (this->thread_id_listener_liveliness_lost_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for 'LIVELINESS_LOST_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_liveliness_lost_== ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: "
                              "Thread switch for 'LIVELINESS_LOST_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                               "listener <%C> - component <%C>\n",
                              list_buf_ll,
                              ccm_buf));
     }
    else
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread switch for 'LIVELINESS_LOST_STATUS' "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_ll,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_liveliness_lost_== ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: 'LIVELINESS_LOST_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_ll,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: 'LIVELINESS_LOST_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_ll,
                              ccm_buf));
      }
    #endif

    //test thread switch for RELIABLE_READER_ACTIVITY_CHANGED_STATUS
    char list_buf_ra [65];
    this->thread_id_reliable_dr_activity_changed_.to_string(list_buf_ra);

    if (this->thread_id_reliable_dr_activity_changed_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for 'RELIABLE_READER_ACTIVITY_CHANGED_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_reliable_dr_activity_changed_== ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: "
                              "Thread switch for 'RELIABLE_READER_ACTIVITY_CHANGED_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf_ra,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread switch for 'RELIABLE_READER_ACTIVITY_CHANGED_STATUS' "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_ra,
                              ccm_buf));
     }
    #else
    else if (this->thread_id_reliable_dr_activity_changed_== ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: 'RELIABLE_READER_ACTIVITY_CHANGED_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                               "listener <%C> - component <%C>\n",
                              list_buf_ra,
                              ccm_buf));
     }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: 'RELIABLE_READER_ACTIVITY_CHANGED_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_ra,
                              ccm_buf));
      }
    #endif

    //test thread switch for RELIABLE_WRITER_CACHE_CHANGED_STATUS
     char list_buf_rw [65];
    this->thread_id_reliable_writer_cache_changed_.to_string (list_buf_rw);

    if (this->thread_id_reliable_writer_cache_changed_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for 'RELIABLE_WRITER_CACHE_CHANGED_STATUS' not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_reliable_writer_cache_changed_== ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: "
                              "Thread switch for 'RELIABLE_WRITER_CACHE_CHANGED_STATUS' seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                               "listener <%C> - component <%C>\n",
                              list_buf_rw,
                              ccm_buf));
    }
    else
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread switch for 'RELIABLE_WRITER_CACHE_CHANGED_STATUS' "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf_rw,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_reliable_writer_cache_changed_== ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: 'RELIABLE_WRITER_CACHE_CHANGED_STATUS': "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_rw,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: 'RELIABLE_WRITER_CACHE_CHANGED_STATUS': "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf_rw,
                              ccm_buf));
      }
    #endif
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_USTest_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
