// $Id$

#include "ContentFilteredTopic_T.h"
#include "Topic_T.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_ContentFilteredTopic_T (
      DDSContentFilteredTopic * dw) : impl_ (dw)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_ContentFilteredTopic_T (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_filter_expression (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_filter_expression");
      return CORBA::string_dup (this->impl ()->get_filter_expression ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_expression_parameters (
      ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_expression_parameters");
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t retval =
        this->impl ()->get_expression_parameters (parameters);
      expression_parameters <<= parameters;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_expression_parameters (
      const ::DDS::StringSeq & expression_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_expression_parameters");

      DDS_StringSeq parameters;
      parameters <<= expression_parameters;
      return this->impl ()->set_expression_parameters (parameters);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::Topic_ptr
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_related_topic (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_related_topic");

      ::DDS::Topic_var retval = ::DDS::Topic::_nil ();
      DDSTopic *topic = this->impl ()->get_related_topic ();
      ACE_NEW_THROW_EX (retval,
                        Topic_type (topic),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_type_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_type_name ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    char *
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_name ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
      DDSDomainParticipant * p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type (p),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (DDSContentFilteredTopic * dw)
    {
      this->impl_ = dw;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSContentFilteredTopic *
    CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
