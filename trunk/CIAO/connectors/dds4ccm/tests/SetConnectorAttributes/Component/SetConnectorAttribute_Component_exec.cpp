// -*- C++ -*-
//
// $Id$

#include "SetConnectorAttribute_Component_exec.h"
#include "Base/SetConnectorAttribute_BaseSupport.h"

#include "dds4ccm/impl/dds/Utils.h"

#define DOMAIN_ID_IN_DP 56
#define PROFILE_IN_DP "SetConnectorAttribute_Profile"
#define LIBRARY_IN_DP "SetConnectorAttribute_Library"
#define TOPIC_NAME_IN_DP "SetConnectorAttribute"

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
  Component_exec_i::check_attributes (DDSDataWriter * dw)
  {
    //check topic name
    DDSTopic * tp = dw->get_topic ();
    if (!tp)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                              "Unable to retrieve topic.\n"));
      }
    else
      {
        if (ACE_OS::strcmp (tp->get_name (), TOPIC_NAME_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_attributes - "
                                  "Topic name set properly.\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                                  "Topic name not properly set: "
                                  "expected <%C> - retrieved <%C>\n",
                                  TOPIC_NAME_IN_DP,
                                  tp->get_name ()));
          }
      }
    //check domain id
    ::DDSDomainParticipant * part = 0;
    ::DDSPublisher * pub = dw->get_publisher ();
    if (!pub)
      {
        ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                              "Unable to retrieve Publisher.\n"));
      }
    else
      {
        
        if (pub->get_domain_id () != DOMAIN_ID_DP)
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_attributes - "
                                  "ERROR: Domain ID not set properly: "
                                  "expected <%u> - retrieved <%u>\n",
                                  DOMAIN_ID_DP,
                                  pub->get_domain_id ()));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_attributes - "
                                  "Domain ID set properly.\n"));
          }
    this->check_profile_names (dw, tp, pub);
  }

  void
  Component_exec_i::check_profile_names (DDSDataWriter * dw,
                                         DDSTopic * tp,
                                         DDSPublisher * pub)
  {
    if (dw)
      {
        if (ACE_OS::strcmp (dw->get_default_profile (), PROFILE_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Profile name set properly in DataWriter\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Profile name not set properly in DataWriter\n"));
          }
        if (ACE_OS::strcmp (dw->get_default_library (), LIBRARY_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Library name set properly in DataWriter\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Library name not set properly in DataWriter\n"));
          }
      }
    if (tp)
      {
        if (ACE_OS::strcmp (tp->get_default_profile (), PROFILE_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Profile name set properly in Topic\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Profile name not set properly in Topic\n"));
          }
        if (ACE_OS::strcmp (tp->get_default_library (), LIBRARY_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Library name set properly in Topic\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Library name not set properly in Topic\n"));
          }
      }
    if (pub)
      {
        if (ACE_OS::strcmp (pub->get_default_profile (), PROFILE_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Profile name set properly in Publisher\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Profile name not set properly in Publisher\n"));
          }
        if (ACE_OS::strcmp (pub->get_default_library (), LIBRARY_IN_DP) == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Component_exec_i::check_profile_names - "
                                  "Library name set properly in Publisher\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Component_exec_i::check_profile_names - "
                                  "ERROR: Library name not set properly in Publisher\n"));
          }
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
    CIAO::DDS4CCM::CCM_DDS_DataWriter_i *ccm_dds_dw =
      dynamic_cast <CIAO::DDS4CCM::CCM_DDS_DataWriter_i *> (dds_dw.in ());
    if (!ccm_dds_dw)
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::ccm_activate - "
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
        ACE_ERROR ((LM_ERROR, "Component_exec_i::ccm_activate - "
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

