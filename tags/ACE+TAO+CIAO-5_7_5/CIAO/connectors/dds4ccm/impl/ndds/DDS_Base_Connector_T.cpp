// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
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
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  CIAO_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  this->configuration_complete_ = true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_domain (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_Base_Connector_T::configure_default_domain_ - "
                "Configuring default domain\n"));

  if (CORBA::is_nil (this->domain_participant_factory_.in ()))
    {
      try
      {
        //NDDSConfigLogger::get_instance()->set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
        //                                                           NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL );

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
            this->domain_participant_factory_->set_default_participant_qos_with_profile (this->library_name_, this->profile_name_);
            this->domain_participant_ =
              this->domain_participant_factory_->create_participant_with_profile (this->domain_id_,
                                                   this->library_name_,
                                                   this->profile_name_,
                                                   0,
                                                   DDS_STATUS_MASK_NONE);
          }
        else
          {
            ::DDS::DomainParticipantQos qos;
            this->domain_participant_ =
              this->domain_participant_factory_->create_participant (this->domain_id_,
                                                         qos,
                                                         0,
                                                         DDS_STATUS_MASK_NONE);
          }
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, "Caught unknown C++ exception while configuring default domain\n"));
        throw CORBA::INTERNAL ();
      }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  CIAO_TRACE ("set_session_context");
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
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
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


