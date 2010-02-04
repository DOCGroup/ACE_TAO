// -*- C++ -*-
//
// $Id$

//Test for on_unexpected_status with status LIVELINESS_CHANGED_STATUS' and 'SUBSCRIPTION_MATCHED_STATUS

#include "CSL_USTest_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"


namespace CIAO_CSL_USTest_Receiver_Impl
{
//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Atomic_Boolean &unexpected_matched,Atomic_Boolean &unexpected_liveliness)
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
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)
  {
    CORBA::ULong kind = status_kind;
    if (!CORBA::is_nil (the_entity) && kind == DDS::SUBSCRIPTION_MATCHED_STATUS)
      {
        this->unexpected_matched_ = true;
      }
    if(!CORBA::is_nil(the_entity) && kind == DDS::LIVELINESS_CHANGED_STATUS)
      {
        this->unexpected_liveliness_ = true;
      }
  }

  //============================================================
  // Facet Executor Implementation Class: TestTopic_RawListener_exec_i
  //============================================================
  TestTopic_RawListener_exec_i::TestTopic_RawListener_exec_i (Atomic_ULong &received)
      : received_ (received)
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
  }

  void
  TestTopic_RawListener_exec_i::on_many_data (
    const TestTopic_Seq & /*an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_RawListener_exec_i ();
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : unexpected_matched_ (false),
      unexpected_liveliness_ (false),
      received_(0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::TestTopic::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_RawListener_exec_i (this->received_);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->unexpected_matched_,this->unexpected_liveliness_);
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
  Receiver_exec_i::configuration_complete (void)
  {
       this->reader_ = this->context_->get_connection_info_out_data ();
       if (CORBA::is_nil (this->reader_))
        {
          ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Reader is null!\n")));
          throw CORBA::INTERNAL ();
        }
   }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();
    if (CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::NOT_ENABLED);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
     if(!this->unexpected_matched_.value () || !this->unexpected_liveliness_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT ("states 'PUBLICATION_MATCHED_STATUS and/or LIVELINESS_CHANGED_STATUS' in Receiver\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'PUBLICATION_MATCHED_STATUS and LIVELINESS_CHANGED_STATUS' in Receiver\n")
                    ));
      }
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
