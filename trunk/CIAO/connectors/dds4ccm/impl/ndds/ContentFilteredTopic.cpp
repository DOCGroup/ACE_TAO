// $Id$

#include "ContentFilteredTopic.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_ContentFilteredTopic_i::RTI_ContentFilteredTopic_i (void)
        : impl_ (0)
      {
      }

      // Implementation skeleton destructor
      RTI_ContentFilteredTopic_i::~RTI_ContentFilteredTopic_i (void)
      {
      }

      char *
      RTI_ContentFilteredTopic_i::get_filter_expression (void)
      {
        CIAO_TRACE ("RTI_ContentFilteredTopic_i::get_filter_expression");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_ContentFilteredTopic_i::get_expression_parameters (
        ::DDS::StringSeq & expression_parameters)
      {
        CIAO_TRACE ("RTI_ContentFilteredTopic_i::get_expression_parameters");
        ACE_UNUSED_ARG (expression_parameters);
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_ContentFilteredTopic_i::set_expression_parameters (
        const ::DDS::StringSeq & expression_parameters)
      {
        CIAO_TRACE ("RTI_ContentFilteredTopic_i::set_expression_parameters");
        ACE_UNUSED_ARG (expression_parameters);
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::Topic_ptr
      RTI_ContentFilteredTopic_i::get_related_topic (void)
      {
        CIAO_TRACE ("RTI_ContentFilteredTopic_i::get_related_topic");
        throw CORBA::NO_IMPLEMENT ();
      }

      char *
      RTI_ContentFilteredTopic_i::get_type_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_type_name ());
      }

      char *
      RTI_ContentFilteredTopic_i::get_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_name ());
      }

      ::DDS::DomainParticipant_ptr
      RTI_ContentFilteredTopic_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl ()->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
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
