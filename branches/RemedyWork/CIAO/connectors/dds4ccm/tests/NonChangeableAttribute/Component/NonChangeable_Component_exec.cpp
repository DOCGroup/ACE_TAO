// -*- C++ -*-
// $Id$

#include "NonChangeable_Component_exec.h"

#include "dds4ccm/impl/Utils.h"

#define DOMAIN_ID_IN_DP 56
#define TOPIC_NAME_IN_DP "NonChangeable"

#define DW_MAX_BLOCKING_TIME_SEC 11
#define DW_MAX_BLOCKING_TIME_NSEC 200

namespace CIAO_NonChangeable_NonChangeableComponent_Impl
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

  bool
  Component_exec_i::test_topic_name (
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn)
  {
    try
      {
        conn->topic_name ("nonchangeable_test");
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "Able to reset topic name without an exception.\n"));
        return true;
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "ERROR: Unable to reset topic name.\n"));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "ERROR: Caught unexpected exception.\n"));
      }
    return false;
  }

  bool
  Component_exec_i::test_key_fields (
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn)
  {
    try
      {
        ::DDS::StringSeq fields;
        fields.length (2);
        fields[0] = CORBA::string_dup ("field_0");
        fields[1] = CORBA::string_dup ("field_1");
        conn->key_fields (fields);
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_domain_id - "
                              "ERROR: Able to set key fields without "
                              "an exception.\n"));
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_key_fields - "
                              "Caught expected exception.\n"));
        return true;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_key_fields - "
                              "ERROR: Caught unexpected exception.\n"));
        return false;
      }
    return false;
  }

  bool
  Component_exec_i::test_domain_id (
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn)
  {
    try
      {
        conn->domain_id (64);
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_domain_id - "
                              "ERROR: Able to set domain ID without "
                              "an exception.\n"));
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_domain_id - "
                              "Caught expected exception.\n"));
        return true;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_domain_id - "
                              "ERROR: Caught unexpected exception.\n"));
        return false;
      }
    return false;
  }

  bool
  Component_exec_i::test_qos_profile (
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn)
  {
    try
      {
        conn->qos_profile ("nonchangeable_profile");
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_qos_profile - "
                              "ERROR: Able to set QoS profile without "
                              "an exception.\n"));
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_qos_profile - "
                              "Caught expected exception.\n"));
        return true;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_qos_profile - "
                              "ERROR: Caught unexpected exception.\n"));
        return false;
      }
    return false;
  }

  bool
  Component_exec_i::test_filter (
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn)
  {
    try
      {
        ::CCM_DDS::QueryFilter filter;
        filter.expression = ::CORBA::string_dup ("na");
        conn->pull_consumer_filter (filter);
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_filter - "
                              "ERROR: Able to set filter without "
                              "an exception.\n"));
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_filter - "
                              "Caught expected exception.\n"));
        return true;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_filter - "
                              "ERROR: Caught unexpected exception.\n"));
        return false;
      }
    return false;
  }

  // Operations from Components::SessionComponent.
  CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_get_status (void)
  {
    return CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::NonChangeable::CCM_NonChangeableComponent_Context::_narrow (ctx);
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
    ::NonChangeable::NonChangeableTestConnector::Reader_var reader =
      this->context_->get_connection_info_get_data ();
    if (::CORBA::is_nil (reader.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Component_exec_i::ccm_activate - "
                              "Unable to get reader interface\n"));
        throw ::CORBA::INTERNAL ();
      }
    ::CORBA::Object_var cmp = reader->_get_component ();
    if (::CORBA::is_nil (cmp.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Component_exec_i::ccm_activate - "
                              "Unable to get component interface\n"));
        throw ::CORBA::INTERNAL ();
      }
    ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_var conn =
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event::_narrow (cmp.in ());
    if (::CORBA::is_nil (conn.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Component_exec_i::ccm_activate - "
                              "Unable to narrow connector interface\n"));
        throw ::CORBA::INTERNAL ();
      }

    if (this->test_topic_name (conn.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Set topic_name test passed.\n"));
      }
    if (this->test_key_fields (conn.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Set key_fields test passed.\n"));
      }
    if (this->test_domain_id (conn.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Set domain_id test passed.\n"));
      }
    if (this->test_qos_profile (conn.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Set qos_profile test passed.\n"));
      }
    if (this->test_filter (conn.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "Set qos_profile test passed.\n"));
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
  create_NonChangeable_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

