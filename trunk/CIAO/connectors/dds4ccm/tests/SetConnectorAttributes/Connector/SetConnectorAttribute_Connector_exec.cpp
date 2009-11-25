// -*- C++ -*-
// $Id$

#include "SetConnectorAttribute_Connector_exec.h"

#define DOMAIN_ID_TO_TEST 100
#define QOS_PROFILE_TO_TEST "'qos profile'"
#define TOPIC_NAME_TO_TEST "SetConnectorAttribute"
#define KEY_FIELDS_PREFIX_TO_TEST "string_"
#define NO_OFF_KEY_FIELDS_TO_TEST 5

namespace CIAO_SetConnectorAttribute_SetConnectorAttribute_Connector_Impl
{
  SetConnectorAttribute_Connector_exec_i::SetConnectorAttribute_Connector_exec_i (void)
    : DDS_Event_Connector_T<SetConnectorAttributeTest_DDS_Traits, SetConnectorAttributeTest_Connector_Traits> ()
  {
  }

  SetConnectorAttribute_Connector_exec_i::~SetConnectorAttribute_Connector_exec_i (void)
  {
  }

  void
  SetConnectorAttribute_Connector_exec_i::test_domain_id ()
  {
    try
      {
        DDS::DomainId_t id = this->domain_id ();
        if (id != DOMAIN_ID_TO_TEST)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                ACE_TEXT ("Domain ID not set properly; set <%d> - should be <%d>\n"),
                id, DOMAIN_ID_TO_TEST));
          }
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
          ACE_TEXT ("ERROR: test_domain_id : Unkown exception caught\n")));
      }
  }

  void
  SetConnectorAttribute_Connector_exec_i::test_qos_profile ()
  {
    try
      {
        char * profile =  this->qos_profile ();
        if (ACE_OS::strcmp (profile, QOS_PROFILE_TO_TEST) != 0)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                ACE_TEXT ("QOS profile not set properly; set <%C> - should be <%C>\n"),
                profile, QOS_PROFILE_TO_TEST));
          }
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
          ACE_TEXT ("ERROR: test_qos_profile : Unkown exception caught\n")));
      }
  }

  void
  SetConnectorAttribute_Connector_exec_i::test_topic_name ()
  {
    try
      {
        char * topic_name =  this->topic_name ();
        if (ACE_OS::strcmp (topic_name, TOPIC_NAME_TO_TEST) != 0)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                ACE_TEXT ("Topic name not set properly; set <%C> - should be <%C>\n"),
                topic_name, TOPIC_NAME_TO_TEST));
          }
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
          ACE_TEXT ("ERROR: test_topic_name : Unkown exception caught\n")));
      }
  }

  void
  SetConnectorAttribute_Connector_exec_i::test_key_fields ()
  {
    try
      {
        ::DDS::StringSeq *key_fields = this->key_fields ();
        if (key_fields->length () != NO_OFF_KEY_FIELDS_TO_TEST)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                ACE_TEXT ("Incorrect size of key fields; set <%C> - should be <%C>\n"),
                key_fields->length (), NO_OFF_KEY_FIELDS_TO_TEST));
          }
        for (CORBA::ULong i = 0; i < key_fields->length (); ++i)
          {
            char tmp[256];
            ACE_OS::sprintf (tmp, KEY_FIELDS_PREFIX_TO_TEST, i+1);
            if (ACE_OS::strcmp (tmp, (*key_fields)[i].in ()) != 0)
              {
                CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                    ACE_TEXT ("Incorrect string found in key fields; ")
                    ACE_TEXT ("index <%d> - set <%C> - should be <%s>\n"),
                    i, (*key_fields)[i].in (), tmp));
              }
          }
        delete key_fields;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
          ACE_TEXT ("ERROR: test_key_fields : Unkown exception caught\n")));
      }
  }
  

  void
  SetConnectorAttribute_Connector_exec_i::ccm_activate ()
  {
    test_domain_id ();
    test_qos_profile ();
    test_topic_name ();
    test_key_fields ();
  }
  
  extern "C" SETCONNECTORATTRIBUTE_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SetConnectorAttribute_SetConnectorAttribute_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      SetConnectorAttribute_Connector_exec_i ());

    return retval;
  }
}

