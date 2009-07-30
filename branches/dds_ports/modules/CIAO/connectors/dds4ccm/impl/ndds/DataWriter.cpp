// $Id$

#include "DataWriter.h"
#include "StatusCondition.h"
#include "Publisher.h"
#include "DataWriterListener.h"
#include "Topic.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"

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
      }

      // Implementation skeleton destructor
      RTI_DataWriter_i::~RTI_DataWriter_i (void)
      {
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
        RTI_DataWriterListener_i *dwl = dynamic_cast< RTI_DataWriterListener_i *> (a_listener);
        if (!dwl)
          {
            return ::DDS::RETCODE_BAD_PARAMETER;
          }
        return this->dw_->set_listener (dwl->get_datawriterlistener (), mask);
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriter_i::get_listener (void)
      {
        DDSDataWriterListener* wr = this->dw_->get_listener ();
        ::DDS::DataWriterListener_var retval = new RTI_DataWriterListener_i (wr);
        return retval._retn ();
      }

      ::DDS::Topic_ptr
      RTI_DataWriter_i::get_topic (void)
      {
        DDSTopic* t = this->dw_->get_topic ();
        ::DDS::Topic_var retval = new RTI_Topic_i (t);
        return retval._retn ();
      }

      ::DDS::Publisher_ptr
      RTI_DataWriter_i::get_publisher (void)
      {
        DDSPublisher* p = this->dw_->get_publisher ();
        ::DDS::Publisher_var retval = new RTI_Publisher_i (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->dw_->wait_for_acknowledgments (rtiduration);
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
        return this->dw_->assert_liveliness ();
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
                                                       const ::DDS::InstanceHandle_t & subscription_handle)
      {
        CIAO_TRACE ("RTI_DataWriter_i::get_matched_subscription_data");
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::enable (void)
      {
        return this->dw_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataWriter_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->dw_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataWriter_i::get_status_changes (void)
      {
        return this->dw_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataWriter_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->dw_->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle; 
      }

      DDSDataWriter *
      RTI_DataWriter_i::get_datawriter (void)
      {
        return this->dw_;
      }
    }
  }
}
