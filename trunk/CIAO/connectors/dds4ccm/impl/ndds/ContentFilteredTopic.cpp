// $Id$

#include "ContentFilteredTopic.h"
#include "Topic.h"
#include "StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_ContentFilteredTopic_i::RTI_ContentFilteredTopic_i (void)
        : impl_ (0)
      {
      }

      RTI_ContentFilteredTopic_i::~RTI_ContentFilteredTopic_i (void)
      {
      }

      char *
      RTI_ContentFilteredTopic_i::get_filter_expression (void)
      {
        DDS4CCM_TRACE ("RTI_ContentFilteredTopic_i::get_filter_expression");
        return CORBA::string_dup (this->impl ()->get_filter_expression ());
      }

      ::DDS::ReturnCode_t
      RTI_ContentFilteredTopic_i::get_expression_parameters (
        ::DDS::StringSeq & expression_parameters)
      {
        DDS4CCM_TRACE ("RTI_ContentFilteredTopic_i::get_expression_parameters");
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        DDS_StringSeq parameters;
        ::DDS::ReturnCode_t retval = this->impl ()->get_expression_parameters (
                                                                    parameters);
        expression_parameters <<= parameters;
#else
        ::DDS::ReturnCode_t retval = this->impl ()->get_expression_parameters (
                                                                    expression_parameters);
#endif
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_ContentFilteredTopic_i::set_expression_parameters (
        const ::DDS::StringSeq & expression_parameters)
      {
        DDS4CCM_TRACE ("RTI_ContentFilteredTopic_i::set_expression_parameters");

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        DDS_StringSeq parameters;
        parameters <<= expression_parameters;
        return this->impl ()->set_expression_parameters (parameters);
#else
        return this->impl ()->set_expression_parameters (expression_parameters);
#endif
      }

      ::DDS::Topic_ptr
      RTI_ContentFilteredTopic_i::get_related_topic (void)
      {
        DDS4CCM_TRACE ("RTI_ContentFilteredTopic_i::get_related_topic");

        ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
      RTI_ContentFilteredTopic_i::get_type_name (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        return CORBA::string_dup (this->impl ()->get_type_name ());
#else
#endif
      }

      char *
      RTI_ContentFilteredTopic_i::get_name (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        return CORBA::string_dup (this->impl ()->get_name ());
#else
#endif
      }

      ::DDS::DomainParticipant_ptr
      RTI_ContentFilteredTopic_i::get_participant (void)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DomainParticipant_i (),
                          CORBA::NO_MEMORY ());

        DDSDomainParticipant* p = this->impl ()->get_participant ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
#else
#endif
      }

      DDSContentFilteredTopic *
      RTI_ContentFilteredTopic_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_ContentFilteredTopic_i::set_impl (DDSContentFilteredTopic * dw)
      {
        this->impl_ = dw;
      }

      DDSContentFilteredTopic *
      RTI_ContentFilteredTopic_i::impl (void)
      {
        if (!this->impl_)
          {
            throw ::CORBA::BAD_INV_ORDER ();
          }
        return this->impl_;
      }

    }
  }
}
