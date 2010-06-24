// -*- C++ -*-
//
// $Id$

#include "SetConnectorAttribute_Component_exec.h"
#include "Base/SetConnectorAttribute_BaseSupport.h"
#include "Connector/SetConnectorAttribute_Connector_conn.h"

#include "dds4ccm/impl/Utils.h"

#define DOMAIN_ID_IN_DP 56
#define TOPIC_NAME_IN_DP "SetConnectorAttribute"

#define DW_MAX_BLOCKING_TIME_SEC 11
#define DW_MAX_BLOCKING_TIME_NSEC 200

namespace CIAO_SetConnectorAttribute_SetConnectorAttributeComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  void
  Component_exec_i::check_topic_name (DDSTopic * tp)
  {
    if (ACE_OS::strcmp (tp->get_name (), TOPIC_NAME_IN_DP) == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_topic_name - "
                              "Topic name set properly.\n"));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_topic_name - "
                              "Topic name not properly set: "
                              "expected <%C> - retrieved <%C>\n",
                              TOPIC_NAME_IN_DP,
                              tp->get_name ()));
      }
  }

  void
  Component_exec_i::check_domain_id (DDSPublisher * pub)
  {
    DDSDomainParticipant * part = pub->get_participant ();
    if (part->get_domain_id () != DOMAIN_ID_IN_DP)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_domain_id - "
                              "ERROR: Domain ID not set properly: "
                              "expected <%u> - retrieved <%u>\n",
                              DOMAIN_ID_IN_DP,
                              part->get_domain_id ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_domain_id - "
                              "Domain ID set properly.\n"));
      }
  }

  void
  Component_exec_i::check_attributes (DDSDataWriter * dw)
  {
    // Check topic name
    DDSTopic * tp = dw->get_topic ();
    if (!tp)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                              "Unable to retrieve topic.\n"));
      }
    else
      {
        this->check_topic_name (tp);
      }
    //check domain id
    ::DDSPublisher * pub = dw->get_publisher ();
    if (!pub)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                              "Unable to retrieve Publisher.\n"));
      }
    else
      {
        this->check_domain_id (pub);
      }
    this->check_profile (pub);
  }

  void
  Component_exec_i::check_profile (DDSPublisher * pub)
  {
    DDS_DataWriterQos dw_qos;
    pub->get_default_datawriter_qos (dw_qos);
    if (dw_qos.reliability.max_blocking_time.sec == DW_MAX_BLOCKING_TIME_SEC &&
        dw_qos.reliability.max_blocking_time.nanosec == DW_MAX_BLOCKING_TIME_NSEC)
      {
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile - "
                              "ERROR: Profile not set properly: "
                              "expected sec <%u> - retrieved sec <%u> and "
                              "expected nanosec <%u> - retrieved nanosec <%u>\n",
                              dw_qos.reliability.max_blocking_time.sec,
                              DW_MAX_BLOCKING_TIME_SEC,
                              dw_qos.reliability.max_blocking_time.nanosec,
                              DW_MAX_BLOCKING_TIME_NSEC));
      }
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SetConnectorAttribute::CCM_SetConnectorAttributeComponent_Context::_narrow (ctx);
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
    //check the settings on the connector
    DDS::DataWriter_var dds_dw =
      this->context_->get_connection_info_write_dds_entity ();

    typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<
      CIAO_SetConnectorAttribute_SetConnectorAttributeTestConnector_DDS_Event_Impl::DDS_DDS_Event_Traits,
      CIAO_SetConnectorAttribute_SetConnectorAttributeTestConnector_DDS_Event_Impl::SetConnectorAttributeTest_DDS_Traits>
        DataWriter_type;

    DataWriter_type * ccm_dds_dw = dynamic_cast < DataWriter_type * > (dds_dw.in ());
    if (!ccm_dds_dw)
      {
        ACE_DEBUG ((LM_DEBUG, "ERROR: Component_exec_i::ccm_activate - "
                              "Unable to cast\n"));
        return;
      }
    DDSDataWriter * dw = ccm_dds_dw->get_impl ();
    if (dw)
      {
        this->check_attributes (dw);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Component_exec_i::ccm_activate - "
                              "Unable to retrieve DDSDataWriter.\n"));
      }
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SetConnectorAttribute_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

