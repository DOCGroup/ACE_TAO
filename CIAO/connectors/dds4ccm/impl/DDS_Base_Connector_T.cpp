// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/DomainParticipant_T.h"
#include "ace/Tokenizer_T.h"
#include "ace/Env_Value_T.h"

#include "dds4ccm/impl/Utils.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
  , configuration_complete_ (false)
  , library_name_ (0)
  , profile_name_ (0)
{
  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), this->domain_id_);
  this->domain_id_ = id;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DDS_Base_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::domain_id (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::domain_id");

  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::domain_id");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->domain_id_ = domain_id;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
char *
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::qos_profile (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::qos_profile");

  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::qos_profile (
  const char * qos_profile)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::qos_profile");

  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->qos_profile_ = qos_profile;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_default_domain (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_default_domain");

  DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "DDS_Base_Connector_T::init_default_domain - "
                "Configuring default domain <%d>\n",
                this->domain_id_));
#if (CIAO_DDS4CCM_NDDS==1)
  ACE_Env_Value<int> verbosity (ACE_TEXT("DDS4CCM_NDDS_LOG_VERBOSITY"),
    NDDS_CONFIG_LOG_VERBOSITY_SILENT);

  NDDS_Config_LogVerbosity n_verbosity =
    static_cast <NDDS_Config_LogVerbosity> (verbosity.operator int());
  NDDSConfigLogger::get_instance()->set_verbosity (n_verbosity);
#endif /* CIAO_DDS4CCM_NDDS==1 */

  // Generic parsing code, library and profile should be seperated by a #
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
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_session_context");

  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if (::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete");
  this->init_default_domain ();
  this->configuration_complete_ = true;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_activate");

  if (::CORBA::is_nil (this->domainparticipantlistener_.in ()))
    {
      ACE_NEW_THROW_EX (this->domainparticipantlistener_,
                        DomainParticipantListener(
                          this->context_->
                            get_connection_error_listener ()),
                        ::CORBA::NO_MEMORY ());
    }
  ::DDS::ReturnCode_t const retcode = this->domain_participant_->set_listener (
                              this->domainparticipantlistener_.in (),
                              DomainParticipantListener::get_mask ());
  if (retcode == DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    "DDS_Base_Connector_T::ccm_activate - "
                    "Error during set_listener - <%C>\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_passivate");

  this->domain_participant_->set_listener (
    ::DDS::DomainParticipantListener::_nil (),
    0);
  this->domainparticipantlistener_ =
    ::DDS::DomainParticipantListener::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_remove");

  ::DDS::ReturnCode_t const retcode = this->dp_factory_.delete_participant (
                                                this->domain_participant_.in ());
  if (retcode != ::DDS::RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }

  this->domain_participant_ = ::DDS::DomainParticipant::_nil ();
}
