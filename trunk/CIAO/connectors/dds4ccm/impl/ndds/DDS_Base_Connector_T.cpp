// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DomainParticipantListener_T.h"
#include "ace/Tokenizer_T.h"
#include "ace/Env_Value_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
  , configuration_complete_ (false)
  , library_name_ (0)
  , profile_name_ (0)
{
  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), this->domain_id_);
  this->domain_id_ = id;
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Base_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id");

  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->domain_id_ = domain_id;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile");

  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->qos_profile_ = qos_profile;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::init_default_domain (void)
{
  CIAO_DEBUG (9, (LM_TRACE, CLINFO "DDS_Base_Connector_T::configure_default_domain_ - "
                "Configuring default domain\n"));

  if (CORBA::is_nil (this->domain_participant_factory_.in ()))
    {
      try
      {
        ACE_Env_Value<int> verbosity (ACE_TEXT("DDS4CCM_NDDS_LOG_VERBOSITY"),
          NDDS_CONFIG_LOG_VERBOSITY_SILENT);

        NDDS_Config_LogVerbosity n_verbosity =
          static_cast <NDDS_Config_LogVerbosity> (verbosity.operator int());
        NDDSConfigLogger::get_instance()->set_verbosity (n_verbosity);

        // Generic code
        this->domain_participant_factory_ =
          new ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i ();

        if (this->qos_profile_.in ())
          {
            char* buf = ACE_OS::strdup (this->qos_profile_.in ());
            ACE_Tokenizer_T<char> tok (buf);
            tok.delimiter_replace ('#', 0);
            for (char *p = tok.next (); p; p = tok.next ())
            {
              if (!this->library_name_)
              {
                this->library_name_ = p;
              }
              else if (!this->profile_name_)
              {
                this->profile_name_ = p;
              }
            }
          }
        if (this->library_name_ && this->profile_name_)
          {
            this->domain_participant_factory_->
              set_default_participant_qos_with_profile (
                this->library_name_,
                this->profile_name_);
            this->domain_participant_ =
              this->domain_participant_factory_->create_participant_with_profile (
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
              this->domain_participant_factory_->create_participant (
                this->domain_id_,
                qos,
                ::DDS::DomainParticipantListener::_nil (),
                0);
          }
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, "DDS_Base_Connector_T::init_default_domain: "
                                  "Caught unknown C++ exception while configuring default domain\n"));
        throw CORBA::INTERNAL ();
      }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::set_session_context");

  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if (::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  this->init_default_domain ();
  this->configuration_complete_ = true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate");
  try
    {
      if (CORBA::is_nil (this->domainparticipantlistener_.in ()))
        {
          this->domainparticipantlistener_ = new ::CIAO::DDS4CCM::DomainParticipantListener_T
            <DDS_TYPE, CCM_TYPE> (
              this->context_->get_connection_error_listener ());
        }
      this->domain_participant_->set_listener (
        this->domainparticipantlistener_.in (),
        ::CIAO::DDS4CCM::DomainParticipantListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_Base_Connector_T::ccm_activate: "
                                "Caught unknown C++ exception while configuring default domain\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate");
  try
    {
      this->domain_participant_->set_listener (
        ::DDS::DomainParticipantListener::_nil (),
        0);
      this->domainparticipantlistener_ = ::DDS::DomainParticipantListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_Base_Connector_T::ccm_passivate: "
                                "Caught unknown C++ exception while configuring default domain\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove");

  if (! ::CORBA::is_nil (this->domain_participant_.in ()))
    {
      this->domain_participant_->delete_contained_entities ();
    }

  if (! ::CORBA::is_nil (this->domain_participant_factory_.in ()))
    {
      this->domain_participant_factory_->delete_participant (
        this->domain_participant_.in ());
      this->domain_participant_ = ::DDS::DomainParticipant::_nil ();
    }
}
