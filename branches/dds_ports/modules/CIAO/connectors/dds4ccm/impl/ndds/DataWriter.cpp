// $Id$

#include "DataWriter.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataWriter_i::RTI_DataWriter_i (DDSDataWriter * dw)
        : dw_ (dw)
      {
        CIAO_TRACE ("RTI_DataWriter_i::RTI_DataWriter_i");
      }

      // Implementation skeleton destructor
      RTI_DataWriter_i::~RTI_DataWriter_i (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::~RTI_DataWriter_i");
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::set_qos (const ::DDS::DataWriterQos & qos)
      {
        CIAO_TRACE ("RTI_DataWriter_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_qos (::DDS::DataWriterQos & qos)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::set_listener (::DDS::DataWriterListener_ptr a_listener,
                                      ::DDS::StatusMask mask)
      {
        
        CIAO_TRACE ("RTI_DataWriter_i::set_listener");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::DataWriterListener_ptr 
      RTI_DataWriter_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_listener");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::Topic_ptr RTI_DataWriter_i::get_topic (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_topic");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::Publisher_ptr 
      RTI_DataWriter_i::get_publisher (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_publisher");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
        CIAO_TRACE ("RTI_DataWriter_i::wait_for_acknowledgments");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_liveliness_lost_status");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_offered_deadline_missed_status");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_offered_incompatible_qos_status");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_publication_matched_status (::DDS::PublicationMatchedStatus & status)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_publication_matched_status");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::assert_liveliness (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::assert_liveliness");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_matched_subscriptions");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t 
      RTI_DataWriter_i::get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & subscription_data,
                                                       ::DDS::InstanceHandle_t subscription_handle)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_matched_subscription_data");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::enable (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::enable");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::StatusCondition_ptr 
      RTI_DataWriter_i::get_statuscondition (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::StatusMask 
      RTI_DataWriter_i::get_status_changes (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_status_changes");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::InstanceHandle_t 
      RTI_DataWriter_i::get_instance_handle (void)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_instance_handle");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }
      
      DDSDataWriter * 
      RTI_DataWriter_i::get_datawriter (void)
      {
        return this->dw_;
      }
    }
  }
}
