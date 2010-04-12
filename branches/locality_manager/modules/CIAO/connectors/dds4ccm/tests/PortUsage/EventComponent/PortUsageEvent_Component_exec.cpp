// -*- C++ -*-
//
// $Id$

#include "PortUsageEvent_Component_exec.h"

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/dds/Utils.h"

namespace CIAO_PortUsageEvent_Test_PortUsageEventComponent_Impl
{
  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
                                    Atomic_ULong& datareaders_count,
                                    Atomic_ULong& datawriters_count)
    : datareaders_count_ (datareaders_count),
      datawriters_count_ (datawriters_count)
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
    ACE_DEBUG ((LM_DEBUG,
              "ConnectorStatusListener_exec_i::on_unexpected_status <%C>\n",
              CIAO::DDS4CCM::translate_statuskind (status_kind)));
    if (status_kind == DDS::PUBLICATION_MATCHED_STATUS)
      {
        DDS::DataWriter_var dw = DDS::DataWriter::_narrow (the_entity);
        if (! ::CORBA::is_nil (dw.in ()))
          {
            ++this->datawriters_count_;
            ACE_DEBUG ((LM_DEBUG, "DataWriters found until now <%u>\n",
                this->datawriters_count_.value ()));
          }
        else
          {
            DDS::DataReader_var dr = DDS::DataReader::_narrow (the_entity);
            if (! ::CORBA::is_nil (dr.in ()))
              {
                ACE_ERROR ((LM_ERROR, "ERROR : DATAREADER isn't nil when receiving an "
                                      "on_publication_matched status\n"));
              }
          }
      }
    if (status_kind == DDS::SUBSCRIPTION_MATCHED_STATUS)
      {
        DDS::DataReader_var dw = DDS::DataReader::_narrow (the_entity);
        if (! ::CORBA::is_nil (dw.in ()))
          {
            ++this->datareaders_count_;
            ACE_DEBUG ((LM_DEBUG, "DataReaders found until now <%u>\n",
                this->datareaders_count_.value ()));
          }
        DDS::DataWriter_var dr = DDS::DataWriter::_narrow (the_entity);
        if (! ::CORBA::is_nil (dr.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR : DATAREADER isn't nil when receiving an "
                                  "on_subscription_matched status\n"));
          }
      }
  }

  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
    : assignment_ (PORTUSAGE_ASSIGNMENT_WRITE),
      datareaders_count_ (0),
      datawriters_count_ (0)
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Component_exec_i::check_writer ()
  {
     if (this->datawriters_count_.value () != 1)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_writer - "
                  "ERROR : Unexpected number of DataWriters detected - "
                  "expected <1> - detected <%u>\n",
                  this->datawriters_count_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_writer - "
                  "OK : Detected the expected number of DataWriters - "
                  "expected <1> - detected <%u>\n",
                  this->datawriters_count_.value ()));
      }
  }

  void
  Component_exec_i::check_getter ()
  {
     if (this->datareaders_count_.value () != 1)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_getter - "
                  "ERROR : Unexpected number of DataReaders detected - "
                  "expected <1> - detected <%u>\n",
                  this->datareaders_count_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_getter - "
                  "OK : Detected the expected number of DataReaders - "
                  "expected <1> - detected <%u>\n",
                  this->datareaders_count_.value ()));
      }
  }

  void
  Component_exec_i::check_listener ()
  {
     if (this->datareaders_count_.value () != 1)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_listener - "
                  "ERROR : Unexpected number of DataReaders detected - "
                  "expected <1> - detected <%u>\n",
                  this->datareaders_count_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_listener - "
                  "OK : Detected the expected number of DataReaders - "
                  "expected <1> - detected <%u>\n",
                  this->datareaders_count_.value ()));
      }
  }

  // Component attributes.
  PortUsageAssignment
  Component_exec_i::portusage_assignment()
  {
    return this->assignment_;
  }

  void
  Component_exec_i::portusage_assignment(PortUsageAssignment assignment)
  {
    switch (assignment)
      {
        case PORTUSAGE_ASSIGNMENT_WRITE:
          ACE_DEBUG ((LM_DEBUG, "New assigment: 'check writer port'\n"));
          break;
        case PORTUSAGE_ASSIGNMENT_GET:
          ACE_DEBUG ((LM_DEBUG, "New assigment: 'check getter port'\n"));
          break;
        case PORTUSAGE_ASSIGNMENT_LISTEN:
          ACE_DEBUG ((LM_DEBUG, "New assigment: 'check listener port'\n"));
          break;
        default:
          ACE_ERROR ((LM_ERROR, "Component_exec_i::portusage_assignment - "
                                "ERROR : Unknown assignment\n"));
          break;

      }
    this->assignment_ = assignment;
  }

  // Port operations.
  CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_get_port_status()
  {
    return CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  PortUsageEvent_Test::PortUsageTestConn::CCM_Listener_ptr
  Component_exec_i::get_listen_port_data_listener()
  {
    return PortUsageEvent_Test::PortUsageTestConn::CCM_Listener::_nil ();
  }

  CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_listen_port_status()
  {
    return CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  CCM_DDS::CCM_ConnectorStatusListener_ptr
  Component_exec_i::get_connector_status ()
  {
    return new ConnectorStatusListener_exec_i (
                  this->datareaders_count_,
                  this->datawriters_count_);
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::PortUsageEvent_Test::CCM_PortUsageEventComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
    switch (this->assignment_)
      {
        case PORTUSAGE_ASSIGNMENT_WRITE:
          this->check_writer ();
          break;
        case PORTUSAGE_ASSIGNMENT_GET:
          this->check_getter ();
          break;
        case PORTUSAGE_ASSIGNMENT_LISTEN:
          this->check_listener ();
          break;
        default:
          ACE_ERROR ((LM_ERROR, "Component_exec_i::ccm_remove - "
                                "ERROR : Unknown assignment\n"));
          break;

      }
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PortUsageEvent_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

