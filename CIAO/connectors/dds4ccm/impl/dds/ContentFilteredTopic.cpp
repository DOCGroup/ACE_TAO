// $Id$

#include "ContentFilteredTopic.h"
#include "Topic.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/impl/dds/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::CCM_DDS_ContentFilteredTopic_i (
      DDSContentFilteredTopic * dw) : impl_ (dw)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::~CCM_DDS_ContentFilteredTopic_i (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_filter_expression (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_filter_expression");
      return CORBA::string_dup (this->impl ()->get_filter_expression ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_expression_parameters (
      ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_expression_parameters");
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t retval =
        this->impl ()->get_expression_parameters (parameters);
      expression_parameters <<= parameters;
#else
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_expression_parameters (expression_parameters);
#endif
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::set_expression_parameters (
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::set_expression_parameters");

#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      parameters <<= expression_parameters;
      return this->impl ()->set_expression_parameters (parameters);
#else
      return this->impl ()->set_expression_parameters (expression_parameters);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_related_topic (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_related_topic");

      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSTopic *topic = this->impl ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (topic),
                        CORBA::NO_MEMORY ());
      return retval._retn ();
#else
      ::DDS::Topic_var topic = this->impl ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (topic.in ()),
                        CORBA::NO_MEMORY ());
      return retval._retn ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_type_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_type_name ());
#else
      return this->impl ()->get_type_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_name ());
#else
      return this->impl ()->get_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant * p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::set_impl (DDSContentFilteredTopic * dw)
    {
      this->impl_ = dw;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
