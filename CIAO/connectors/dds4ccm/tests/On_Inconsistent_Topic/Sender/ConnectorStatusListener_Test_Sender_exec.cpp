// -*- C++ -*-
// $Id$

#include "ConnectorStatusListener_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_ConnectorStatusListener_Test_Sender_Impl
{

  //============================================================
  // ConnectorStatusListener_sec_exec_i
  //============================================================
  ConnectorStatusListener_sec_exec_i::ConnectorStatusListener_sec_exec_i (Atomic_Boolean &inconsistent,
                                                                          Atomic_ThreadId &thread_id)
    : inconsistent_ (inconsistent),
      thread_id_ (thread_id)
  {
  }

  ConnectorStatusListener_sec_exec_i::~ConnectorStatusListener_sec_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_sec_exec_i::on_inconsistent_topic (
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/)
  {
    this->thread_id_ = ACE_Thread::self ();
    this->inconsistent_ = true;
  }

  void ConnectorStatusListener_sec_exec_i::on_requested_incompatible_qos (
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_sec_exec_i::on_sample_rejected (
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_sec_exec_i::on_offered_deadline_missed (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_sec_exec_i::on_offered_incompatible_qos (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_sec_exec_i::on_unexpected_status (
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)
  {
  }

  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : inconsistent_ (false),
      thread_id_listener_ (0)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_sec_topic_connector_status (void)
  {
    return new ConnectorStatusListener_sec_exec_i (this->inconsistent_,
                                                   this->thread_id_listener_);
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ConnectorStatusListener_Test::CCM_Sender_Context::_narrow (ctx);

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
  Sender_exec_i::add_instance_of_sec_topic (const char * key, int x)
  {
    TestSecondTopic *new_key = new TestSecondTopic;
    new_key->key = CORBA::string_dup(key);
    new_key->x = x;
    new_key->y = x + 10;
    this->sec_ktests_[key] = new_key;
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    //add 2 different instances of topic
    this->add_instance_of_sec_topic ("ONE",1);
    this->add_instance_of_sec_topic ("TWO",2);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (this->inconsistent_.value ())
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SENDER OK: Received the expected ")
                              ACE_TEXT ("'on_inconsistent_topic'\n")
                    ));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("SENDER ERROR: did not receive the expected ")
                              ACE_TEXT ("error 'on_inconsistent_topic'\n")
                    ));
      }
    if (this->thread_id_listener_.value () == 0)
      {
        ACE_ERROR ((LM_ERROR, "SENDER ERROR: "
                              "Thread ID for ConnectorStatusListener not set!\n"));
      }
    #if defined (CIAO_DDS4CCM_CONTEXT_SWITCH) && (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
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
  create_ConnectorStatusListener_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

