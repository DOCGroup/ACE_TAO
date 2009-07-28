// $Id$

#include "Subscriber.h"
#include "Topic.h"
#include "DataWriter.h"
#include "Utils.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_Subscriber_i::RTI_Subscriber_i (DDSSubscriber *s)
        : sub_ (s)
      {
        CIAO_TRACE ("RTI_Subscriber_i::RTI_Subscriber_i");
      }

      // Implementation skeleton destructor
      RTI_Subscriber_i::~RTI_Subscriber_i (void)
      {
        CIAO_TRACE ("RTI_Subscriber_i::~RTI_Subscriber_i");
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::enable (void)
      {
        return this->sub_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Subscriber_i::get_statuscondition (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::StatusMask
      RTI_Subscriber_i::get_status_changes (void)
      {
        return this->sub_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Subscriber_i::get_instance_handle (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::create_datareader (
        ::DDS::TopicDescription_ptr a_topic,
        const ::DDS::DataReaderQos & qos,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_datareader (
        ::DDS::DataReader_ptr a_datareader)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_contained_entities (void)
      {
        return this->sub_->delete_contained_entities ();
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::lookup_datareader (
        const char * topic_name)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_datareaders (
        ::DDS::DataReaderSeq & readers,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::notify_datareaders (void)
      {
        return this->sub_->notify_datareaders ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_qos (
        const ::DDS::SubscriberQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_qos (
        ::DDS::SubscriberQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::SubscriberListener_ptr
      RTI_Subscriber_i::get_listener (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::begin_access (void)
      {
        return this->sub_->begin_access ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::end_access (void)
      {
        return this->sub_->end_access ();
      }

      ::DDS::DomainParticipant_ptr
      RTI_Subscriber_i::get_participant (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_default_datareader_qos (
        const ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_default_datareader_qos (
        ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::copy_from_topic_qos (
        ::DDS::DataReaderQos & a_datareader_qos,
        const ::DDS::TopicQos & a_topic_qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSSubscriber *
      RTI_Subscriber_i::get_subscriber (void)
      {
        return this->sub_;
      }
    }
  }
}

