/**
 * @file DataReader.h
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Simple IDL facade on top of NDDS.
 */
#ifndef CIAO_RTI_DATAREADER_H
#define CIAO_RTI_DATAREADER_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"
#include "tao/LocalObject.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DataReader_i
        : public virtual ::DDS::CCM_DataReader,
          public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        RTI_DataReader_i (DDSDataReader *dr);

        // Destructor
        virtual ~RTI_DataReader_i (void);

        virtual ::DDS::ReturnCode_t
        enable (void);

        virtual ::DDS::StatusCondition_ptr
        get_statuscondition (void);

        virtual ::DDS::StatusMask
        get_status_changes (void);

        virtual ::DDS::InstanceHandle_t
        get_instance_handle (void);

        virtual ::DDS::ReadCondition_ptr
        create_readcondition (
          ::DDS::SampleStateMask sample_states,
          ::DDS::ViewStateMask view_states,
          ::DDS::InstanceStateMask instance_states);

        virtual ::DDS::QueryCondition_ptr
        create_querycondition (
          ::DDS::SampleStateMask sample_states,
          ::DDS::ViewStateMask view_states,
          ::DDS::InstanceStateMask instance_states,
          const char * query_expression,
          const ::DDS::StringSeq & query_parameters);

        virtual ::DDS::ReturnCode_t
        delete_readcondition (
          ::DDS::ReadCondition_ptr a_condition);

        virtual ::DDS::ReturnCode_t
        delete_contained_entities (void);

        virtual ::DDS::ReturnCode_t
        set_qos (
          const ::DDS::DataReaderQos & qos);

        virtual ::DDS::ReturnCode_t
        get_qos (
          ::DDS::DataReaderQos & qos);

        virtual ::DDS::ReturnCode_t
        set_listener (
          ::DDS::DataReaderListener_ptr a_listener,
          ::DDS::StatusMask mask);

        virtual ::DDS::DataReaderListener_ptr
        get_listener (void);

        virtual ::DDS::TopicDescription_ptr
        get_topicdescription (void);

        virtual ::DDS::Subscriber_ptr
        get_subscriber (void);

        virtual ::DDS::ReturnCode_t
        get_sample_rejected_status (
          ::DDS::SampleRejectedStatus & status);

        virtual ::DDS::ReturnCode_t
        get_liveliness_changed_status (
          ::DDS::LivelinessChangedStatus & status);

        virtual ::DDS::ReturnCode_t
        get_requested_deadline_missed_status (
          ::DDS::RequestedDeadlineMissedStatus & status);

        virtual ::DDS::ReturnCode_t
        get_requested_incompatible_qos_status (
          ::DDS::RequestedIncompatibleQosStatus & status);

        virtual ::DDS::ReturnCode_t
        get_subscription_matched_status (
          ::DDS::SubscriptionMatchedStatus & status);

        virtual ::DDS::ReturnCode_t
        get_sample_lost_status (
          ::DDS::SampleLostStatus & status);

        virtual ::DDS::ReturnCode_t
        wait_for_historical_data (
          const ::DDS::Duration_t & max_wait);

        virtual ::DDS::ReturnCode_t
        get_matched_publications (
          ::DDS::InstanceHandleSeq & publication_handles);

        virtual ::DDS::ReturnCode_t
        get_matched_publication_data (
          ::DDS::PublicationBuiltinTopicData & publication_data,
          const ::DDS::InstanceHandle_t & publication_handle);

        DDSDataReader * get_datareader (void);

      private:
        DDSDataReader * dr_;
      };
    }
  }
}

#endif
