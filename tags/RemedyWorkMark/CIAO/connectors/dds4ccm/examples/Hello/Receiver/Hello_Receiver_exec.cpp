// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1344

#include "Hello_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"

namespace CIAO_Hello_Receiver_Impl
{

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exs.cpp:75
  //============================================================
  // Facet Executor Implementation Class: info_out_data_listener_exec_i
  //============================================================

  info_out_data_listener_exec_i::info_out_data_listener_exec_i (
        ::Hello::CCM_Receiver_Context_ptr ctx,
        Atomic_ULong &received,
        const ACE_CString &name)
    : ciao_context_ (
        ::Hello::CCM_Receiver_Context::_duplicate (ctx)),
      received_ (received),
      name_ (name)

  {
  }

  info_out_data_listener_exec_i::~info_out_data_listener_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~info_out_data_listener_exec_i\n")));
  }

  // Operations from ::Hello::Listener

  void
  info_out_data_listener_exec_i::on_one_data (
    const ::DDSHello & datum,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    ACE_CString rec (datum.hello.in ());
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
                    datum.hello.in (),
                    datum.iterator,
                    now.microsec () - usec_rec));
      }
    else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("<%C> received <%C> - <%d>.\n"),
      this->name_.c_str (),
      datum.hello.in (),
      datum.iterator));
    }
  }

  void
  info_out_data_listener_exec_i::on_many_data (
    const ::DDSHelloSeq & /* data */,
    const ::CCM_DDS::ReadInfoSeq & /* infos */)
  {
    /* Your code here. */
  }

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exs.cpp:75
  //============================================================
  // Facet Executor Implementation Class: info_out_status_exec_i
  //============================================================

  info_out_status_exec_i::info_out_status_exec_i (
        ::Hello::CCM_Receiver_Context_ptr ctx,
        Atomic_ULong &lost)
    : ciao_context_ (
        ::Hello::CCM_Receiver_Context::_duplicate (ctx)),
      lost_ (lost)
  {
  }

  info_out_status_exec_i::~info_out_status_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~info_out_status_exec_i\n")));
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  info_out_status_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  info_out_status_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
    ++this->lost_;
    ACE_DEBUG ((LM_ERROR, ACE_TEXT("port status listener::on_sample_lost\n")));
  }

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exs.cpp:75
  //============================================================
  // Facet Executor Implementation Class: info_out_connector_status_exec_i
  //============================================================

  info_out_connector_status_exec_i::info_out_connector_status_exec_i (
        ::Hello::CCM_Receiver_Context_ptr ctx)
    : ciao_context_ (
        ::Hello::CCM_Receiver_Context::_duplicate (ctx))
  {
  }

  info_out_connector_status_exec_i::~info_out_connector_status_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~info_out_connector_status_exec_i\n")));
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener

  void
  info_out_connector_status_exec_i::on_inconsistent_topic (
    ::DDS::Topic_ptr /* the_topic */,
    const ::DDS::InconsistentTopicStatus & /* status */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_inconsistent_topic\n")));
  }

  void
  info_out_connector_status_exec_i::on_requested_incompatible_qos (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedIncompatibleQosStatus & /* status */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n")));
  }

  void
  info_out_connector_status_exec_i::on_sample_rejected (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleRejectedStatus & /* status */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_sample_rejected\n")));
  }

  void
  info_out_connector_status_exec_i::on_offered_deadline_missed (
    ::DDS::DataWriter_ptr /* the_writer */,
    const ::DDS::OfferedDeadlineMissedStatus & /* status */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n")));
  }

  void
  info_out_connector_status_exec_i::on_offered_incompatible_qos (
    ::DDS::DataWriter_ptr /* the_writer */,
    const ::DDS::OfferedIncompatibleQosStatus & /* status */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n")));
  }

  void
  info_out_connector_status_exec_i::on_unexpected_status (
    ::DDS::Entity_ptr /* the_entity */,
    ::DDS::StatusKind /* status_kind */)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver: ")
          ACE_TEXT ("ConnectorStatusListener_exec_i::on_unexpected_status\n")));
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : expected_ (0),
      received_ (0),
      lost_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~Receiver_exec_i\n")));
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::Hello::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new DDSHello RAW listener\n")));
    if ( ::CORBA::is_nil (this->ciao_info_out_data_listener_.in ()))
      {
        info_out_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_out_data_listener_exec_i (
            this->ciao_context_.in (),
            this->received_,
            this->name_),
          ::Hello::CCM_Listener::_nil ());

        this->ciao_info_out_data_listener_ = tmp;
      }

    return
      ::Hello::CCM_Listener::_duplicate (
        this->ciao_info_out_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    if ( ::CORBA::is_nil (this->ciao_info_out_status_.in ()))
      {
        info_out_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_out_status_exec_i (
            this->ciao_context_.in (),
            this->lost_),
          ::CCM_DDS::CCM_PortStatusListener::_nil ());

        this->ciao_info_out_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_PortStatusListener::_duplicate (
        this->ciao_info_out_status_.in ());
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new ConnectorStatusListener\n")));
    if ( ::CORBA::is_nil (this->ciao_info_out_connector_status_.in ()))
      {
        info_out_connector_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_out_connector_status_exec_i (
            this->ciao_context_.in ()),
          ::CCM_DDS::CCM_ConnectorStatusListener::_nil ());

        this->ciao_info_out_connector_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_ConnectorStatusListener::_duplicate (
        this->ciao_info_out_connector_status_.in ());
  }

  char *
  Receiver_exec_i::name (void)
  {
    return CORBA::string_dup (this->name_.c_str ());
  }

  void
  Receiver_exec_i::name (
    const char * name)
  {
    this->name_ = name;
  }

  ::CORBA::ULong
  Receiver_exec_i::expected_samples (void)
  {
    return this->expected_;
  }

  void
  Receiver_exec_i::expected_samples (
    ::CORBA::ULong expected_samples)
  {
    this->expected_ = expected_samples;
  }

  // Operations from Components::SessionComponent.

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Hello::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
      this->ciao_context_->get_connection_info_out_data_control ();

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
    /* Your code here. */
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
