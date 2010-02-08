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
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &unexpected_pub_matched,
                                                                  Atomic_Boolean &unexpected_sub_matched,
                                                                  Atomic_Boolean &unexpected_liveliness)
   : unexpected_pub_matched_ (unexpected_pub_matched),
     unexpected_sub_matched_ (unexpected_sub_matched),
     unexpected_liveliness_ (unexpected_liveliness)
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
    ACE_DEBUG ((LM_DEBUG, "SENDER ConnectorStatusListener_exec_i::on_unexpected_status: "
                          "received <%C>\n",
                          CIAO::DDS4CCM::translate_statuskind (status_kind)));
    if (!CORBA::is_nil(the_entity) && status_kind == DDS::SUBSCRIPTION_MATCHED_STATUS)
      {
        this->unexpected_sub_matched_ = true;
      }
    else if (!CORBA::is_nil(the_entity) && status_kind == DDS::PUBLICATION_MATCHED_STATUS)
      {
        this->unexpected_pub_matched_ = true;
      }
    else if (!CORBA::is_nil(the_entity) && status_kind == DDS::LIVELINESS_CHANGED_STATUS)
      {
        this->unexpected_liveliness_ = true;
      }
    }
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : unexpected_pub_matched_ (false),
      unexpected_sub_matched_ (false),
      unexpected_liveliness_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->unexpected_pub_matched_,
                                               this->unexpected_sub_matched_,
                                               this->unexpected_liveliness_);
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
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if(!this->unexpected_pub_matched_.value () ||
       !this->unexpected_sub_matched_.value () ||
       !this->unexpected_liveliness_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                              ACE_TEXT ("states 'PUBLICATION_MATCHED_STATUS ")
                              ACE_TEXT ("and/or 'SUBSCRIPTION_MATCHED_STATUS' ")
                              ACE_TEXT ("and/or 'LIVELINESS_CHANGED_STATUS' ")
                              ACE_TEXT ("in Sender\n")
                    ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                              ACE_TEXT ("'LIVELINESS_CHANGED_STATUS' ")
                              ACE_TEXT ("and 'PUBLICATION_MATCHED_STATUS' ")
                              ACE_TEXT ("and 'SUBSCRIPTION_MATCHED_STATUS' ")
                              ACE_TEXT ("in Sender\n")
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
