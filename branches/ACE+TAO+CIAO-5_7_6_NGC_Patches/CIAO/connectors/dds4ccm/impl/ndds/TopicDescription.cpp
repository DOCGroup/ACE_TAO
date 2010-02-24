// $Id$

#include "TopicDescription.h"
#include "DomainParticipant.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_TopicDescription_i::RTI_TopicDescription_i (void)
        : impl_ (0)
      {
      }

      // Implementation skeleton destructor
      RTI_TopicDescription_i::~RTI_TopicDescription_i (void)
      {
      }

      char *
      RTI_TopicDescription_i::get_type_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_type_name ());
      }

      char *
      RTI_TopicDescription_i::get_name (void)
      {
        return CORBA::string_dup (this->impl ()->get_name ());
      }

      ::DDS::DomainParticipant_ptr
      RTI_TopicDescription_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl ()->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
      }

      DDSTopicDescription *
      RTI_TopicDescription_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_TopicDescription_i::set_impl (DDSTopicDescription * rc)
      {
        this->impl_ = rc;
      }

      DDSTopicDescription *
      RTI_TopicDescription_i::impl (void)
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

