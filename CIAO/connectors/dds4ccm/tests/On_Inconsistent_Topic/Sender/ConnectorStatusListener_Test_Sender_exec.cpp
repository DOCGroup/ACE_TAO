// -*- C++ -*-
// $Id$

#include "ConnectorStatusListener_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_ConnectorStatusListener_Test_Sender_Impl
{

  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_sec_exec_i
  //============================================================

  ConnectorStatusListener_sec_exec_i::ConnectorStatusListener_sec_exec_i (Atomic_Boolean &inconsistent)
   : inconsistent_ (inconsistent)
  {
  }

  ConnectorStatusListener_sec_exec_i::~ConnectorStatusListener_sec_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_sec_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/)
    {
//    printf("Sender : ConnectorStatusListener_sec_exec_i::on_inconsistent_topic\n");
      this->inconsistent_ = true;
    }
  void ConnectorStatusListener_sec_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
//      printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n");
    }
  void ConnectorStatusListener_sec_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)  {
//      printf("ConnectorStatusListener_exec_i::on_sample_rejected\n");
    }
  void ConnectorStatusListener_sec_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
 //     printf("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_sec_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
 //     printf("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_sec_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)  {
 //     printf("ConnectorStatusListener_exec_i::on_unexpected_status\n");
    }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : inconsistent_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_sec_topic_connector_status (void)
  {
    //printf ("*************** out connector status sender test_sec_topic************************\n");
    return new ConnectorStatusListener_sec_exec_i (this->inconsistent_);
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
    //printf("-------------configuration_complete ----------------\n");
  }


void
  Sender_exec_i::add_instance_of_sec_topic (const char * key, int x)
  {
    //printf("-------------add_instance_of_second_topic----------------\n");
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
    this->add_instance_of_sec_topic ("EEN",1);
    this->add_instance_of_sec_topic ("TWEE",2);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    //printf("*************in remove Sender********** \n");
    if(!this->inconsistent_.value ())
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT ("error 'on_inconsistent_topic' in Sender\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT (" 'on_inconsistent_topic' in Sender\n")));
      }
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

