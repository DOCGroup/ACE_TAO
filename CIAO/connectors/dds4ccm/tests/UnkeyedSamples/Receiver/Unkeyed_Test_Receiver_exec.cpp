// -*- C++ -*-
//
// $Id$

#include "Unkeyed_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Unkeyed_Test_Receiver_Impl
{
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i ()
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /* the_topic */,
     const DDS::InconsistentTopicStatus & /* status */)
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_inconsistent_topic\n")));
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n")));
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)  
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_sample_rejected\n")));
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n")));
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n")));
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)  
  {
    CIAO_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_unexpected_status\n")));
  }

  //============================================================
  // Facet Executor Implementation Class: UnkeyedTest_Listener_exec_i
  //============================================================

  UnkeyedTest_Listener_exec_i::UnkeyedTest_Listener_exec_i (Atomic_ULong &received)
      : received_ (received)
  {
  }

  UnkeyedTest_Listener_exec_i::~UnkeyedTest_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::UnkeyedTest_Listener

  void
  UnkeyedTest_Listener_exec_i::on_one_data (
    const UnkeyedTest & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("UnkeyedTest_Listener: ")
            ACE_TEXT ("received keyed_test_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.iteration));
  }

  void
  UnkeyedTest_Listener_exec_i::on_many_data (
    const UnkeyedTest_Seq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iUnkeyedTest_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      received_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  // Port operations.
  ::CCM_DDS::UnkeyedTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new UnkeyedTest RAW listener\n")));
    return new UnkeyedTest_Listener_exec_i (this->received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return 0;
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new Connector Status Listener\n")));
    return new ConnectorStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Unkeyed_Test::CCM_Receiver_Context::_narrow (ctx);
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

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    CIAO_DEBUG ((LM_INFO, "Receiver_exec_i summary: received <%u>\n",
                this->received_));
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

