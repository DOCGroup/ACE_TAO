// $Id$

#include "Topic.h"
#include "StatusCondition.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_Topic_i::RTI_Topic_i (DDSTopic * top)
        : topic_ (top)
      {
        CIAO_TRACE ("RTI_Topic_i::RTI_Topic_i");
      }

      // Implementation skeleton destructor
      RTI_Topic_i::~RTI_Topic_i (void)
      {
        CIAO_TRACE ("RTI_Topic_i::~RTI_Topic_i");
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_qos (const ::DDS::TopicQos & qos)
      {
        CIAO_TRACE ("RTI_Topic_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_qos (::DDS::TopicQos & qos)
      {
        CIAO_TRACE ("RTI_Topic_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_listener (::DDS::TopicListener_ptr a_listener,
                                 ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Topic_i::set_listener");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::TopicListener_ptr
      RTI_Topic_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_Topic_i::get_listener");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_inconsistent_topic_status (::DDS::InconsistentTopicStatus & a_status)
      {
        CIAO_TRACE ("RTI_Topic_i::get_inconsistent_topic_status");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::enable (void)
      {
        return this->topic_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Topic_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->topic_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Topic_i::get_status_changes (void)
      {
        return this->topic_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Topic_i::get_instance_handle (void)
      {
        CIAO_TRACE ("RTI_Topic_i::get_instance_handle");
        throw CORBA::NO_IMPLEMENT ();
      }

      char *
      RTI_Topic_i::get_type_name (void)
      {
  //      return this->topic_->get_type_name ();
  return 0;
      }

      char *
      RTI_Topic_i::get_name (void)
      {
//        return this->topic_->get_name ();
return 0;
      }

      ::DDS::DomainParticipant_ptr
      RTI_Topic_i::get_participant (void)
      {
        CIAO_TRACE ("RTI_Topic_i::get_participant");
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSTopic *
      RTI_Topic_i::get_topic (void)
      {
        return topic_;
      }
    }
  }
}
