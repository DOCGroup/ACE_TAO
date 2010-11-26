/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Common dataReader class for the Getter, Reader,
 * and, Listen port
 *
 */
#ifndef DATAREADER_T_H_
#define DATAREADER_T_H_

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"

namespace CIAO
{
  namespace NDDS
  {
    template <typename TYPED_DDS_READER, typename TYPED_READER_TYPE, typename VALUE_TYPE, typename SEQ_TYPE, typename RTI_SEQ_TYPE>
    class DataReader_T
      : public virtual DDS_DataReader_Base,
        public virtual TYPED_READER_TYPE
    {
    public:
      /// Constructor
      explicit DataReader_T (::DDSDataReader * dr,
                             ::DDS::DomainParticipant_ptr dp,
                             ::DDS::Subscriber_ptr sub);

      /// Destructor
      virtual ~DataReader_T (void);

      virtual ::DDS::ReturnCode_t read (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t take (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t read_w_condition (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        ::DDS::ReadCondition_ptr a_condition);

      virtual ::DDS::ReturnCode_t take_w_condition (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        ::DDS::ReadCondition_ptr a_condition);

      virtual ::DDS::ReturnCode_t read_next_sample (
        VALUE_TYPE & data_values,
        ::DDS::SampleInfo & sample_info);

      virtual ::DDS::ReturnCode_t take_next_sample (
        VALUE_TYPE & data_values,
        ::DDS::SampleInfo & sample_info);

      virtual ::DDS::ReturnCode_t read_instance (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & a_handle,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t take_instance (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & a_handle,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t read_next_instance (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & previous_handle,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t take_next_instance (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & previous_handle,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t read_next_instance_w_condition (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & previous_handle,
        ::DDS::ReadCondition_ptr a_condition);

      virtual ::DDS::ReturnCode_t take_next_instance_w_condition (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        const ::DDS::InstanceHandle_t & previous_handle,
        ::DDS::ReadCondition_ptr a_condition);

      virtual ::DDS::ReturnCode_t return_loan (
        SEQ_TYPE & data_values,
        ::DDS::SampleInfoSeq & sample_infos);

      virtual ::DDS::ReturnCode_t get_key_value (
        VALUE_TYPE & key_holder,
        const ::DDS::InstanceHandle_t & handle);

      virtual ::DDS::InstanceHandle_t lookup_instance (
        const VALUE_TYPE& an_instance);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual DDS_INSTANCE_HANDLE_T_RETN get_instance_handle (void);

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

      virtual ::DDS::ReturnCode_t delete_contained_entities (void);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::DataReaderQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (::DDS::DataReaderQos & qos);

      virtual ::DDS::ReturnCode_t
      set_listener (::DDS::DataReaderListener_ptr a_listener,
                    ::DDS::StatusMask mask);

      virtual ::DDS::DataReaderListener_ptr get_listener (void);

      virtual ::DDS::TopicDescription_ptr get_topicdescription (void);

      virtual ::DDS::Subscriber_ptr get_subscriber (void);

      virtual ::DDS::ReturnCode_t
      get_sample_rejected_status (::DDS::SampleRejectedStatus & status);

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
      get_sample_lost_status (::DDS::SampleLostStatus & status);

      virtual ::DDS::ReturnCode_t
      wait_for_historical_data (const ::DDS::Duration_t & max_wait);

      virtual ::DDS::ReturnCode_t
      get_matched_publications (
        ::DDS::InstanceHandleSeq & publication_handles);

      virtual ::DDS::ReturnCode_t
      get_matched_publication_data (
        ::DDS::PublicationBuiltinTopicData & publication_data,
        DDS_INSTANCE_HANDLE_T_IN publication_handle);

      void set_rti_entity (::DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp);

      ::DDS::StatusMask get_mask (void);

      ::DDSDataReader * get_rti_entity (void);

    private:
      TYPED_DDS_READER * rti_entity_;
      ::DDS::DomainParticipant_var dp_;
      ::DDS::Subscriber_var sub_;
      ::DDS::StatusMask lst_mask_;

      TYPED_DDS_READER * rti_entity (void);

      void complete_read (
        RTI_SEQ_TYPE & dds_data_values,
        SEQ_TYPE & data_values,
        DDS_SampleInfoSeq & dds_sample_infos,
        ::DDS::SampleInfoSeq & sample_infos,
        const ::DDS::ReturnCode_t & retcode,
        const char * method_name);
    };
  }
}

#include "dds4ccm/impl/ndds/DataReader_T.cpp"

#endif /* DATAREADER_T_H_ */
