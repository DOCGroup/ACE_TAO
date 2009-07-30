// $Id$

#include "DataReader.h"
#include "DataReaderListener.h"
#include "StatusCondition.h"
#include "ReadCondition.h"
#include "SampleLostStatus.h"
#include "Duration_t.h"
#include "SubscriptionMatchedStatus.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataReader_i::RTI_DataReader_i (DDSDataReader * dr)
        : dr_ (dr)
      {
      }

      // Implementation skeleton destructor
      RTI_DataReader_i::~RTI_DataReader_i (void)
      {
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::enable (void)
      {
        return this->dr_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_DataReader_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->dr_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_DataReader_i::get_status_changes (void)
      {
        return this->dr_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_DataReader_i::get_instance_handle (void)
      {
        //return this->dr_->get_instance_handle ();
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReadCondition_ptr
      RTI_DataReader_i::create_readcondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
      {
        DDSReadCondition* rc = this->dr_->create_readcondition (sample_states, view_states, instance_states);
        ::DDS::ReadCondition_var retval = new RTI_ReadCondition_i (rc);
        return retval._retn ();
      }

      ::DDS::QueryCondition_ptr
      RTI_DataReader_i::create_querycondition (
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states,
        const char * query_expression,
        const ::DDS::StringSeq & query_parameters)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
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
        return this->dr_->delete_readcondition (rc->get_readcondition ());
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::delete_contained_entities (void)
      {
        return this->dr_->delete_contained_entities ();
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
        RTI_DataReaderListener_i *drl = dynamic_cast< RTI_DataReaderListener_i *> (a_listener);
        if (!drl)
          {
            return ::DDS::RETCODE_BAD_PARAMETER;
          }
        //return this->dr_->set_listener (drl->get_datareaderlistener (), mask);
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::DataReaderListener_ptr
      RTI_DataReader_i::get_listener (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::TopicDescription_ptr
      RTI_DataReader_i::get_topicdescription (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::Subscriber_ptr
      RTI_DataReader_i::get_subscriber (void)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_rejected_status (
        ::DDS::SampleRejectedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        //return this->dr_->get_sample_rejected_status (status);
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_liveliness_changed_status (
        ::DDS::LivelinessChangedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_deadline_missed_status (
        ::DDS::RequestedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_requested_incompatible_qos_status (
        ::DDS::RequestedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_subscription_matched_status (
        ::DDS::SubscriptionMatchedStatus & status)
      {
        ::DDS_SubscriptionMatchedStatus ddsstatus;
        ::DDS::ReturnCode_t const retval = this->dr_->get_subscription_matched_status (ddsstatus);
        ddsstatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_sample_lost_status (
        ::DDS::SampleLostStatus & status)
      {
        DDS_SampleLostStatus rtistatus;
        ::DDS::ReturnCode_t const retval = this->dr_->get_sample_lost_status (rtistatus);
        rtistatus >>= status;
        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::wait_for_historical_data (
        const ::DDS::Duration_t & max_wait)
      {
       ::DDS_Duration_t rtiduration;
       rtiduration <<= max_wait;
       return this->dr_->wait_for_historical_data (rtiduration);
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publications (
        ::DDS::InstanceHandleSeq & publication_handles)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_DataReader_i::get_matched_publication_data (
        ::DDS::PublicationBuiltinTopicData & publication_data,
        ::DDS::InstanceHandle_t publication_handle)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSDataReader *
      RTI_DataReader_i::get_datareader (void)
      {
        return this->dr_;
      }
    }
  }
}
