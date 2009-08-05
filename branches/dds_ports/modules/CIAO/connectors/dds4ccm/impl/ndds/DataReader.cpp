// $Id$

#include "DataReader.h"
#include "DataReaderListener.h"
#include "StatusCondition.h"
#include "ReadCondition.h"
#include "QueryCondition.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"
#include "InstanceHandleSeq.h"
#include "Subscriber.h"
#include "TopicDescription.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataReader_i::RTI_DataReader_i (DDSDataReader * dr)
        : impl_ (dr)
      {
      }

      // Implementation skeleton destructor
      RTI_DataReader_i::~RTI_DataReader_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::enable (void)
      {
        return this->impl_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataReader_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataReader_i::get_status_changes (void)
      {
        return this->impl_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataReader_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl_->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      ::DDS::ReadCondition_ptr
      RTI_DataReader_i::create_readcondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
      {
        DDSReadCondition* rc = this->impl_->create_readcondition (sample_states, view_states, instance_states);
        ::DDS::ReadCondition_var retval = new RTI_ReadCondition_i (rc);
        return retval._retn ();
      }

      ::DDS::QueryCondition_ptr
      RTI_DataReader_i::create_querycondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states,
        const char * query_expression,
        const ::DDS::StringSeq & /*query_parameters*/)
      {
        DDSQueryCondition* qc = 0; // @todo = this->impl_->create_querycondition (sample_states, view_states, instance_states, query_expression, query_parameters);
        ::DDS::QueryCondition_var retval = new RTI_QueryCondition_i (qc);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::delete_readcondition (
        ::DDS::ReadCondition_ptr a_condition)
      {
        RTI_ReadCondition_i *rc = dynamic_cast< RTI_ReadCondition_i *> (a_condition);
        if (!rc)
          {
            return ::DDS::RETCODE_BAD_PARAMETER;
          }
        return this->impl_->delete_readcondition (rc->get_readcondition ());
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::delete_contained_entities (void)
      {
        return this->impl_->delete_contained_entities ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::set_qos (
        const ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_qos (
        ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::set_listener (
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        RTI_DataReaderListener_i* rti_impl_list = new RTI_DataReaderListener_i (a_listener);
        return this->impl_->set_listener (rti_impl_list, mask);
      }

      ::DDS::DataReaderListener_ptr
      RTI_DataReader_i::get_listener (void)
      {
        ::DDSDataReaderListener* reader = this->impl_->get_listener ();
//        ::DDS::DataReaderListener_var dds_reader = new RTI_DataReaderListener_i (reader);
        //return dds_reader._retn ();
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::TopicDescription_ptr
      RTI_DataReader_i::get_topicdescription (void)
      {
        ::DDSTopicDescription* reader = this->impl_->get_topicdescription ();
        ::DDS::TopicDescription_var dds_td = new RTI_TopicDescription_i (reader);
        return dds_td._retn ();
      }

      ::DDS::Subscriber_ptr
      RTI_DataReader_i::get_subscriber (void)
      {
        ::DDSSubscriber* subscriber = this->impl_->get_subscriber ();
        ::DDS::Subscriber_var dds_td = new RTI_Subscriber_i (subscriber);
        return dds_td._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_rejected_status (
        ::DDS::SampleRejectedStatus & status)
      {
        DDS_SampleRejectedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_sample_rejected_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_liveliness_changed_status (
        ::DDS::LivelinessChangedStatus & status)
      {
        DDS_LivelinessChangedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_liveliness_changed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_deadline_missed_status (
        ::DDS::RequestedDeadlineMissedStatus & status)
      {
        DDS_RequestedDeadlineMissedStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_requested_deadline_missed_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_incompatible_qos_status (
        ::DDS::RequestedIncompatibleQosStatus & status)
      {
        DDS_RequestedIncompatibleQosStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_requested_incompatible_qos_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_subscription_matched_status (
        ::DDS::SubscriptionMatchedStatus & status)
      {
        ::DDS_SubscriptionMatchedStatus ddsstatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_subscription_matched_status (ddsstatus);
        ddsstatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_lost_status (
        ::DDS::SampleLostStatus & status)
      {
        DDS_SampleLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->impl_->get_sample_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::wait_for_historical_data (
        const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->impl_->wait_for_historical_data (rtiduration);
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publications (
        ::DDS::InstanceHandleSeq & publication_handles)
      {
        ::DDS_InstanceHandleSeq rtiseq;
        ::DDS::ReturnCode_t const retval = this->impl_->get_matched_publications (rtiseq);
        rtiseq >>= publication_handles;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publication_data (
        ::DDS::PublicationBuiltinTopicData & publication_data,
        const ::DDS::InstanceHandle_t & publication_handle)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSDataReader *
      RTI_DataReader_i::get_datareader (void)
      {
        return this->impl_;
      }
    }
  }
}
