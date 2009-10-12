// $Id$

#include "Topic.h"
#include "StatusCondition.h"
#include "InstanceHandle_t.h"
#include "InconsistentTopicStatus.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_Topic_i::RTI_Topic_i (DDSTopic * top)
        : impl_ (top)
      {
      }

      // Implementation skeleton destructor
      RTI_Topic_i::~RTI_Topic_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_qos (const ::DDS::TopicQos & /*qos*/)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_qos (::DDS::TopicQos & /*qos*/)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::set_listener (::DDS::TopicListener_ptr /*a_listener*/,
                                 ::DDS::StatusMask /*mask*/)
      {
        //RTI_TopicListener_i* rti_impl_list = new RTI_TopicListener_i (a_listener);
        //return this->impl_->set_listener (rti_impl_list, mask);
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::TopicListener_ptr
      RTI_Topic_i::get_listener (void)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::get_inconsistent_topic_status (::DDS::InconsistentTopicStatus & a_status)
      {
        DDS_InconsistentTopicStatus ddsstatus;
        ddsstatus <<= a_status;
        return this->impl_->get_inconsistent_topic_status (ddsstatus);
      }

      ::DDS::ReturnCode_t
      RTI_Topic_i::enable (void)
      {
        return this->impl_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Topic_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Topic_i::get_status_changes (void)
      {
        return this->impl_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Topic_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl_->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      char *
      RTI_Topic_i::get_type_name (void)
      {
        return CORBA::string_dup (this->impl_->get_type_name ());
      }

      char *
      RTI_Topic_i::get_name (void)
      {
        return CORBA::string_dup (this->impl_->get_name ());
      }

      ::DDS::DomainParticipant_ptr
      RTI_Topic_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl_->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i (p);
        return retval._retn ();
      }

      DDSTopic *
      RTI_Topic_i::get_topic (void)
      {
        return impl_;
      }
    }
  }
}
