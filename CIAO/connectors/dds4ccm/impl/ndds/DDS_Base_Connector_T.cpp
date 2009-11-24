// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "ace/Tokenizer_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
  ,  configuration_complete_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Base_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (void)
{
  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
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
  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
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
  this->configuration_complete_ = true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_domain (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_Base_Connector_T::configure_default_domain_ - "
                "Configuring default domain\n"));

  if (CORBA::is_nil (this->domain_factory_.in ()))
    {
      try
      {
        //NDDSConfigLogger::get_instance()->set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
        //                                                           NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL );

        // Generic code
        ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i* df =
          new ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i ();
        this->domain_factory_ = df;

        const char* library = 0;
        const char* profile = 0;
        char* buf = ACE_OS::strdup (this->qos_profile_.in ());
        ACE_Tokenizer_T<char> tok (buf);
        tok.delimiter_replace ('#', 0);
        for (char *p = tok.next (); p; p = tok.next ())
        {
          if (!library)
          {
            library = p;
          }
          else if (!profile)
          {
            profile = p;
          }
        }
        if (library && profile)
          {
            this->domain_ =
              df->create_participant_with_profile (this->domain_id_,
                                                   library,
                                                   profile,
                                                   0,
                                                   DDS_STATUS_MASK_NONE);
          }
        else
          {
            ::DDS::DomainParticipantQos qos;
            this->domain_ =
              this->domain_factory_->create_participant (this->domain_id_,
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



