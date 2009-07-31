// $Id$

#include "DataWriter.h"
#include "StatusCondition.h"
#include "Publisher.h"
#include "DataWriterListener.h"
#include "Topic.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"
#include "PublicationMatchedStatus.h"
#include "LivelinessLostStatus.h"
#include "OfferedIncompatibleQosStatus.h"
#include "OfferedDeadlineMissedStatus.h"
#include "InstanceHandleSeq.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataWriter_i::RTI_DataWriter_i (DDSDataWriter * dw)
        : impl_ (dw)
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
        RTI_DataWriterListener_i* rti_impl_list = new RTI_DataWriterListener_i (a_listener);
        return this->impl_->set_listener (rti_impl_list, mask);
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriter_i::get_listener (void)
      {
        DDSDataWriterListener* wr = this->impl_->get_listener ();
        RTI_DataWriterListener_i *dwl = dynamic_cast< RTI_DataWriterListener_i *> (wr);
        return dwl->get_datawriterlistener ();
      }

      ::DDS::Topic_ptr
      RTI_DataWriter_i::get_topic (void)
      {
        DDSTopic* t = this->impl_->get_topic ();
        ::DDS::Topic_var retval = new RTI_Topic_i (t);
        return retval._retn ();
      }

      ::DDS::Publisher_ptr
      RTI_DataWriter_i::get_publisher (void)
      {
        DDSPublisher* p = this->impl_->get_publisher ();
        ::DDS::Publisher_var retval = new RTI_Publisher_i (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->impl_->wait_for_acknowledgments (rtiduration);
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_liveliness_lost_status (::DDS::LivelinessLostStatus & status)
      {
        ::DDS_LivelinessLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_liveliness_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status)
      {
        ::DDS_OfferedDeadlineMissedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_offered_deadline_missed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status)
      {
        ::DDS_OfferedIncompatibleQosStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_offered_incompatible_qos_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_publication_matched_status (::DDS::PublicationMatchedStatus & status)
      {
        ::DDS_PublicationMatchedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_publication_matched_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::assert_liveliness (void)
      {
        return this->impl_->assert_liveliness ();
      }

      ::DDS::ReturnCode_t
      RTI_DataWriter_i::get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl_->get_matched_subscriptions (rtiseq);
        rtiseq >>= subscription_handles;
        return retval;
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
        return this->impl_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataWriter_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataWriter_i::get_status_changes (void)
      {
        return this->impl_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataWriter_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl_->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      DDSDataWriter *
      RTI_DataWriter_i::get_datawriter (void)
      {
        return this->impl_;
      }
    }
  }
}
