// $Id$

#include "dds4ccm/impl/ndds/TopicDescription.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_TopicDescription_i::DDS_TopicDescription_i (
      ::DDSTopicDescription * rc,
      ::DDS::DomainParticipant_ptr dp) :
      rti_entity_ (rc),
      dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
    }

    DDS_TopicDescription_i::~DDS_TopicDescription_i (void)
    {
    }

    char *
    DDS_TopicDescription_i::get_type_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_type_name ());
    }

    char *
    DDS_TopicDescription_i::get_name (void)
    {
      return CORBA::string_dup (this->rti_entity ()->get_name ());
    }

    ::DDS::DomainParticipant_ptr
    DDS_TopicDescription_i::get_participant (void)
    {
      return ::DDS::DomainParticipant::_duplicate (this->dp_.in ());
    }

    ::DDSTopicDescription *
    DDS_TopicDescription_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    ::DDSTopicDescription *
    DDS_TopicDescription_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_TopicDescription_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}

