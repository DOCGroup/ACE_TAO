// $Id$

#include "TopicDescription.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_TopicDescription_i::RTI_TopicDescription_i (DDSTopicDescription *td)
        : td_ (td)
      {
      }

      // Implementation skeleton destructor
      RTI_TopicDescription_i::~RTI_TopicDescription_i (void)
      {
      }

      char *
      RTI_TopicDescription_i::get_type_name (void)
      {
        return CORBA::string_dup (this->td_->get_type_name ());
      }

      char *
      RTI_TopicDescription_i::get_name (void)
      {
        return CORBA::string_dup (this->td_->get_name ());
      }

      ::DDS::DomainParticipant_ptr
      RTI_TopicDescription_i::get_participant (void)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSTopicDescription *
      RTI_TopicDescription_i::get_topicdescription (void)
      {
        return this->td_;
      }
    }
  }
}

