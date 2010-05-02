                    // -*- C++ -*-
// $Id$

#include "NonChangeable_Connector_exec.h"

namespace CIAO_NonChangeable_NonChangeable_Connector_Impl
{
  NonChangeable_Connector_exec_impl::NonChangeable_Connector_exec_impl (void)
  {
  }

  NonChangeable_Connector_exec_impl::~NonChangeable_Connector_exec_impl (void)
  {
  }

  bool
  NonChangeable_Connector_exec_impl::test_topic_name ()
  {
    try
      {
        this->topic_name ("nonchangeable_test");
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "ERROR: Able to set topic name without "
                              "an exception.\n"));
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_DEBUG ((LM_DEBUG, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "Caught expected exception.\n"));
        return true;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "NonChangeable_Connector_exec_impl_i::test_topic_name - "
                              "ERROR: Caught unexpected exception.\n"));
        return false;
      }
    return false;
  }

  bool
  NonChangeable_Connector_exec_impl::test_key_fields ()
  {
    try
      {
        ::DDS::StringSeq fields;
        fields.length (2);
        fields[0] = CORBA::string_dup ("field_0");
        fields[1] = CORBA::string_dup ("field_1");
        this->key_fields (fields);
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
  NonChangeable_Connector_exec_impl::test_domain_id ()
  {
    try
      {
        this->domain_id (64);
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
  NonChangeable_Connector_exec_impl::test_qos_profile ()
  {
    try
      {
        this->qos_profile ("nonchangeable_profile");
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

  void
  NonChangeable_Connector_exec_impl::ccm_activate (void)
  {
    NonChangeable_Connector_exec_i::ccm_activate ();
    if (this->test_topic_name ())
      {
        ACE_DEBUG ((LM_DEBUG, "Set topic_name test passed.\n"));
      }
    if (this->test_key_fields ())
      {
        ACE_DEBUG ((LM_DEBUG, "Set key_fields test passed.\n"));
      }
    if (this->test_domain_id ())
      {
        ACE_DEBUG ((LM_DEBUG, "Set domain_id test passed.\n"));
      }
    if (this->test_qos_profile ())
      {
        ACE_DEBUG ((LM_DEBUG, "Set qos_profile test passed.\n"));
      }
  }

  extern "C" NONCHANGEABLE_CONNECTOR_CONN_Export ::Components::EnterpriseComponent_ptr
  create_NonChangeable_NonChangeable_Connector_i (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();
    ACE_NEW_NORETURN (
      retval,
      NonChangeable_Connector_exec_impl);
    return retval;
  }
}
