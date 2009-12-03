// -*- C++ -*-
// $Id$


//Test for on_unexpected_status with status LIVELINESS_CHANGED_STATUS' and 'PUBLICATION_MATCHED_STATUS
#include "CSL_USTest_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "dds4ccm/impl/ndds/Utils.h"

namespace CIAO_CSL_USTest_Sender_Impl
{

  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================

  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &unexpected_matched, Atomic_Boolean &unexpected_liveliness)
   : unexpected_matched_ (unexpected_matched),
     unexpected_liveliness_ (unexpected_liveliness)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {

  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/)
    {
     //printf("Sender : ConnectorStatusListener_exec_i::on_inconsistent_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
     // printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n");

    }
  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_sample_rejected\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
     // printf("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
     // printf("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  status_kind)  {
      CORBA::ULong kind = status_kind;
      if(kind==DDS::PUBLICATION_MATCHED_STATUS)
      {
        this->unexpected_matched_ = true;
      }
      if(kind==DDS::LIVELINESS_CHANGED_STATUS)
      {
        this->unexpected_liveliness_ = true;
      }
      //printf("Sender :CSL:on_unexpected_status status_kind = %d %s \n", kind, ::CIAO::DDS4CCM::translate_statuskind (kind));
    }
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : unexpected_matched_ (false),
      unexpected_liveliness_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->unexpected_matched_,this->unexpected_liveliness_);
  }

  // Supported operations and attributes.


  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CSL_USTest::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->writer_  = this->context_->get_connection_test_topic_write_data ();
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
    this->add_instance_of_topic ("EEN",1);
    this->add_instance_of_topic ("TWEE",2);
 }



  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if(!this->unexpected_matched_.value () || !this->unexpected_liveliness_.value ())
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT ("states 'PUBLICATION_MATCHED_STATUS and/or LIVELINESS_CHANGED_STATUS' in Sender\n")
                    ));
      }

    else
      {
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'LIVELINESS_CHANGED_STATUS' and 'PUBLICATION_MATCHED_STATUS' in Sender\n")
                    ));
      }
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

