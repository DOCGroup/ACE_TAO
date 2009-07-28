// $Id$

#include "Subscriber.h"
#include "SubscriberListener.h"
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
        DDS_SubscriberQos rti_sub_qos;
/*        rti_sub_qos.presentation = qos.presentation;
        rti_sub_qos.partition = qos.partition;
        rti_sub_qos.group_data = qos.group_data;
        rti_sub_qos.entity_factory = qos.entity_factory;*/
        return this->sub_->set_qos (rti_sub_qos);
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_qos (
        ::DDS::SubscriberQos & qos)
      {
        DDS_SubscriberQos rti_sub_qos;
        DDS_ReturnCode_t const rti_retcode = this->sub_->get_qos (rti_sub_qos);
        /*qos.presentation = rti_sub_qos.presentation;
        qos.partition = rti_sub_qos.partition;
        qos.group_data = rti_sub_qos.group_data;
        qos.entity_factory = rti_sub_qos.entity_factory;*/
        return rti_retcode;
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        RTI_SubscriberListener_i* rti_sub_list = dynamic_cast <RTI_SubscriberListener_i*>(a_listener);
        
        if (!rti_sub_list)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Subscriber_i::set_listener "
                         "Unable to cast provided subscriber listener to servant\n"));
            throw CORBA::INTERNAL ();
          }
        
        return this->sub_->set_listener (rti_sub_list->get_subscriber_listener (), mask);
      }

      ::DDS::SubscriberListener_ptr
      RTI_Subscriber_i::get_listener (void)
      {
        DDSSubscriberListener* rti_sub_list = this->sub_->get_listener ();
        return new RTI_SubscriberListener_i (rti_sub_list);
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

