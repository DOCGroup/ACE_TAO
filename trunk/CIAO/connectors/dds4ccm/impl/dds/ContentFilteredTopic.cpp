// $Id$

#include "ContentFilteredTopic.h"
#include "Topic.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_ContentFilteredTopic_i::CCM_DDS_ContentFilteredTopic_i (
      DDSContentFilteredTopic * dw) : impl_ (dw)
    {
    }

    CCM_DDS_ContentFilteredTopic_i::~CCM_DDS_ContentFilteredTopic_i (void)
    {
    }

    char *
    CCM_DDS_ContentFilteredTopic_i::get_filter_expression (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i::get_filter_expression");
      return CORBA::string_dup (this->impl ()->get_filter_expression ());
    }

    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_i::get_expression_parameters (
      ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i::get_expression_parameters");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t retval =
        this->impl ()->get_expression_parameters (parameters);
      expression_parameters <<= parameters;
#else
      ::DDS::ReturnCode_t retval =
        this->impl ()->get_expression_parameters (expression_parameters);
#endif
      return retval;
    }

    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_i::set_expression_parameters (
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i::set_expression_parameters");

#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      parameters <<= expression_parameters;
      return this->impl ()->set_expression_parameters (parameters);
#else
      return this->impl ()->set_expression_parameters (expression_parameters);
#endif
    }

    ::DDS::Topic_ptr
    CCM_DDS_ContentFilteredTopic_i::get_related_topic (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i::get_related_topic");

      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSTopic *topic = this->impl ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_i (topic),
                        CORBA::NO_MEMORY ());
      return retval._retn ();
#else
      ::DDS::Topic_var topic = this->impl ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_Topic_i (topic.in ()),
                        CORBA::NO_MEMORY ());
      return retval._retn ();
#endif
    }

    char *
    CCM_DDS_ContentFilteredTopic_i::get_type_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_type_name ());
#else
      return this->impl ()->get_type_name ();
#endif
    }

    char *
    CCM_DDS_ContentFilteredTopic_i::get_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_name ());
#else
      return this->impl ()->get_name ();
#endif
    }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_ContentFilteredTopic_i::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_ContentFilteredTopic_i::set_impl (DDSContentFilteredTopic * dw)
    {
      this->impl_ = dw;
    }

    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
