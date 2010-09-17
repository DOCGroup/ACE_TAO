// -*- C++ -*-
// $Id$

#include "Hello_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"

namespace CIAO_Hello_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: DDSHello_Listener_exec_i
  //============================================================

  DDSHello_Listener_exec_i::DDSHello_Listener_exec_i (Atomic_ULong &received,
                                                      const ACE_CString &name)
      : received_ (received),
        name_ (name)
  {
  }

  DDSHello_Listener_exec_i::~DDSHello_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::DDSHello_Listener

  void
  DDSHello_Listener_exec_i::on_many_data (
    const DDSHelloSeq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  DDSHello_Listener_exec_i::on_one_data (
    const DDSHello & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    ACE_CString rec (an_instance.hello.in ());
    ACE_Date_Time now;
    int const sec_rec = ACE_OS::atoi (rec.substr (0, 2).c_str() );
    if (sec_rec > 0)
      {
        int usec_rec = ACE_OS::atoi (rec.substr (3, 6).c_str ());
        if (sec_rec != now.second ())
          {
            usec_rec += 10000000;
          }
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("<%C> received <%C> - <%d>. difference <%d>\n"),
                    this->name_.c_str (),
                    an_instance.hello.in (),
                    an_instance.iterator,
                    now.microsec () - usec_rec));
      }
    else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("<%C> received <%C> - <%d>.\n"),
      this->name_.c_str (),
      an_instance.hello.in (),
      an_instance.iterator));
    }
  }
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (Atomic_ULong &lost)
    : lost_ (lost)
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
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
    ++this->lost_;
    ACE_DEBUG ((LM_ERROR, ACE_TEXT("port status listener::on_sample_lost\n")));
  }


  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================

  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (void)
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
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_inconsistent_topic\n")));
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n")));
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_sample_rejected\n")));
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n")));
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n")));
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  /*status_kind*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_unexpected_status\n")));
  }

  void ConnectorStatusListener_exec_i::on_subscription_matched (
    ::DDS::DataReader_ptr /*the_reader*/,
    const ::DDS::SubscriptionMatchedStatus & /*status*/)
  {
    ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_subscription_matched\n")));
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iDDSHello_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : expected_ (0),
      received_ (0),
      lost_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::CORBA::ULong
  Receiver_exec_i::expected_samples (void)
  {
    return this->expected_;
  }

  void
  Receiver_exec_i::expected_samples (::CORBA::ULong expected_samples)
  {
    this->expected_ = expected_samples;
  }

  char *
  Receiver_exec_i::name (void)
  {
    return CORBA::string_dup (this->name_.c_str ());
  }

  void
  Receiver_exec_i::name (const char *name)
  {
    this->name_ = name;
  }

  // Port operations.
  ::Hello::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new DDSHello RAW listener\n")));
    return new DDSHello_Listener_exec_i (this->received_, this->name_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i (this->lost_);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new ConnectorStatusListener\n")));
    return new ConnectorStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::Hello::CCM_Receiver_Context::_narrow (ctx);
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
        ACE_ERROR ((LM_INFO,
                    ACE_TEXT ("Error:  Listener control receptacle is null!\n")));

        throw ::CORBA::INTERNAL ();
      }
    lc->mode ( ::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    ACE_DEBUG ((LM_INFO,
                "Receiver_exec_i summary: received <%u> - expected <%d> - lost <%u>\n",
                this->received_.value (),
                this->expected_,
                this->lost_.value ()));
    if (this->lost_ > 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Lost %u samples\n",
            this->lost_.value ()));
      }
    if (this->received_ < this->expected_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Expected to receive %u samples, actually got %u\n",
            this->expected_, this->received_.value ()));
      }
  }

  extern "C" HELLO_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}




