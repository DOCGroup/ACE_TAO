// $Id$

#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/ContentFilteredTopic.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/convertors/StringSeq.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_ContentFilteredTopic_i::DDS_ContentFilteredTopic_i (
        ::DDSContentFilteredTopic * cft,
        ::DDS::DomainParticipant_ptr dp)
      : rti_entity_ (cft),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
    }

    DDS_ContentFilteredTopic_i::~DDS_ContentFilteredTopic_i (void)
    {
    }

    char *
    DDS_ContentFilteredTopic_i::get_filter_expression (void)
    {
      DDS4CCM_TRACE ("DDS_ContentFilteredTopic_i::get_filter_expression");
      return CORBA::string_dup (this->rti_entity ()->get_filter_expression ());
    }

    ::DDS::ReturnCode_t
    DDS_ContentFilteredTopic_i::get_expression_parameters (
      ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_ContentFilteredTopic_i::get_expression_parameters");
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t retval =
        this->rti_entity ()->get_expression_parameters (parameters);
      expression_parameters <<= parameters;
      return retval;
    }

    ::DDS::ReturnCode_t
    DDS_ContentFilteredTopic_i::set_expression_parameters (
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("DDS_ContentFilteredTopic_i::set_expression_parameters");

      DDS_StringSeq parameters;
      parameters <<= expression_parameters;
      return this->rti_entity ()->set_expression_parameters (parameters);
    }

    ::DDS::Topic_ptr
    DDS_ContentFilteredTopic_i::get_related_topic (void)
    {
      DDS4CCM_TRACE ("DDS_ContentFilteredTopic_i::get_related_topic");

      ::DDS::Topic_var retval;
      DDSTopic *topic = this->rti_entity ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        DDS_Topic_i (topic, this->dp_.in ()),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    char *
    DDS_ContentFilteredTopic_i::get_type_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_type_name ());
    }

    char *
    DDS_ContentFilteredTopic_i::get_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_name ());
    }

    ::DDS::DomainParticipant_ptr
    DDS_ContentFilteredTopic_i::get_participant (void)
    {
      return ::DDS::DomainParticipant::_duplicate (this->dp_.in ());
    }

    DDSContentFilteredTopic *
    DDS_ContentFilteredTopic_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_ContentFilteredTopic_i::set_rti_entity (DDSContentFilteredTopic * cft)
    {
      this->rti_entity_ = cft;
    }

    DDSContentFilteredTopic *
    DDS_ContentFilteredTopic_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_ContentFilteredTopic_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}
