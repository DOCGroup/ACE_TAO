// -*- C++ -*-
// $Id$

#include "ace/Tokenizer_T.h"
#include "ace/Env_Value_T.h"
#include "tao/ORB_Core.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
  , configuration_complete_ (false)
  , library_name_ (0)
  , profile_name_ (0)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::DDS_Base_Connector_T");

  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), this->domain_id_);
  this->domain_id_ = id;
  this->dlf_ = ACE_Dynamic_Service<CIAO::DDS4CCM::Logger_Service>::instance ("DDS4CCM_Logger");
  this->dlf_->init ();

}

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::~DDS_Base_Connector_T (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::~DDS_Base_Connector_T");

  ACE_OS::free (library_name_);
  ACE_OS::free (profile_name_);

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::~DDS_Base_Connector_T - "
                "Connector has been destructed\n"));
}

template <typename CCM_TYPE, typename DDS_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::domain_id (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::domain_id");

  return this->domain_id_;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::domain_id");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->domain_id_ = domain_id;
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
char *
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::qos_profile (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::qos_profile");

  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::qos_profile (
  const char * qos_profile)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::qos_profile");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->qos_profile_ = qos_profile;
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::init_default_domain (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::init_default_domain");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                "DDS_Base_Connector_T::init_default_domain - "
                "Start configuring default domain <%d>\n",
                this->domain_id_));

  // Generic parsing code, library and profile should be separated by a #
  if (this->qos_profile_.in ())
    {
      char* buf = ACE_OS::strdup (this->qos_profile_.in ());
      ACE_Tokenizer_T<char> tok (buf);
      tok.delimiter_replace ('#', 0);
      for (char *p = tok.next (); p; p = tok.next ())
      {
        if (!this->library_name_)
        {
          this->library_name_ = ACE_OS::strdup (p);
        }
        else if (!this->profile_name_)
        {
          this->profile_name_ = ACE_OS::strdup (p);
        }
      }
      ACE_OS::free (buf);
    }
  if (this->library_name_ && this->profile_name_)
    {
      this->dp_factory_.set_default_participant_qos_with_profile (
          this->library_name_,
          this->profile_name_);
      this->domain_participant_ =
        this->dp_factory_.create_participant_with_profile (
            this->domain_id_,
            this->library_name_,
            this->profile_name_,
            ::DDS::DomainParticipantListener::_nil (),
            0);
    }
  else
    {
      ::DDS::DomainParticipantQos qos;
      this->domain_participant_ =
        this->dp_factory_.create_participant (
          this->domain_id_,
          qos,
          ::DDS::DomainParticipantListener::_nil (),
          0);
    }

  if (::CORBA::is_nil (this->domain_participant_.in ()))
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Base_Connector_T::init_default_domain - "
                  "Error: Unable to create DomainParticipant for domain <%d>\n",
                  this->domain_id_));
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }

}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::set_session_context");

  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if (::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete");
  this->init_default_domain ();
  this->configuration_complete_ = true;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate");
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate");
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove");

  ::DDS::ReturnCode_t const retcode = this->dp_factory_.delete_participant (
                                                this->domain_participant_.in ());
  if (retcode != ::DDS::RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }

  this->domain_participant_ = ::DDS::DomainParticipant::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
ACE_Reactor*
DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::reactor (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<CCM_TYPE, DDS_TYPE>::reactor");

  ACE_Reactor* reactor = 0;

  ::CORBA::Object_var ccm_object = this->context_->get_CCM_object();
  if (!::CORBA::is_nil (ccm_object.in ()))
    {
      ::CORBA::ORB_var orb = ccm_object->_get_orb ();
      if (!::CORBA::is_nil (orb.in ()))
        {
          reactor = orb->orb_core ()->reactor ();
        }
    }

  return reactor;
}

