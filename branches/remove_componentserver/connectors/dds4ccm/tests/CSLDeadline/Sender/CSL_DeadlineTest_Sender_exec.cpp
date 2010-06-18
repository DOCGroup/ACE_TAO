// -*- C++ -*-
// $Id$

// Test for OFFERED_DEADLINE_MISSED status : Writer failed to write data within the deadline time period set in the profile.

#include "CSL_DeadlineTest_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_CSL_DeadlineTest_Sender_Impl
{

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
    Atomic_Boolean &deadline_missed,
    Atomic_ThreadId &thread_id)
    : deadline_missed_ (deadline_missed),
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
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
    ::DDS::DataWriter_ptr the_writer,
    const DDS::OfferedDeadlineMissedStatus & status)
  {
    this->thread_id_ = ACE_Thread::self ();
    if(status.last_instance_handle.isValid && ! ::CORBA::is_nil (the_writer))
      {
        this->deadline_missed_ = true;
      }
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)
  {
  }

  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : deadline_missed_ (false),
      thread_id_listener_ (0)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->deadline_missed_,
                                               this->thread_id_listener_);
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CSL_DeadlineTest::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::write (void)
  {
    ::CSLDeadlineConnector::Writer_var writer =
      this->context_->get_connection_test_topic_write_data ();

    if (! ::CORBA::is_nil (writer.in ()) )
      {
        //to force an 'offered_deadline_missed'  write the topics with a pause of 2 sec in between and
        //in the profile the deadline is set to 1 sec.
        for (CSL_QoSTest_Table::iterator i = this->_ktests_.begin ();
            i != this->_ktests_.end ();
            ++i)
          {
            try
              {
                ACE_OS::sleep (2);
                ::DDS::InstanceHandle_t const hnd =
                  writer->register_instance (i->second);
                writer->write_one(i->second,hnd);
              }
            catch (const CCM_DDS::InternalError& )
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("Internal Error while writing topic for <%C>.\n"),
                            i->first.c_str ()));
              }
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

  void
  Sender_exec_i::ccm_activate (void)
  {
    //add 2 different instances of topic
    this->add_instance_of_topic ("ONE",1);
    this->add_instance_of_topic ("TWO",2);

    //write the instances once for the test
    this->write();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (this->deadline_missed_.value ())
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                              ACE_TEXT ("'on_offered_deadline_missed'\n")));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: did not receive the expected ")
                              ACE_TEXT ("error 'on_offered_deadline_missed'\n")));
      }
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for ConnectorStatusListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (ACE_OS::thr_equal (this->thread_id_listener_.value (),
                                ACE_Thread::self ()))
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: "
                              "Thread switch for ConnectorStatusListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
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
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: ConnectorStatusListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SENDER OK: ConnectorStatusListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%u> - component <%u>\n",
                              this->thread_id_listener_.value (),
                              ACE_Thread::self ()));
      }
    #endif
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_DeadlineTest_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
