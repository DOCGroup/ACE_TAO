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

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_Topic_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_Subscriber_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_ReadCondition_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_QueryCondition_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_ContentFilteredTopic_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_DataReaderListener_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DataReader_T
      : public virtual DDS_DataReader_Base
    {
    typedef DDS_DataReaderListener_T<DDS_TYPE, VENDOR_TYPE> DataReaderListener_type;
    typedef DDS_Topic_T<DDS_TYPE, VENDOR_TYPE> Topic_type;
    typedef DDS_ReadCondition_T<DDS_TYPE, VENDOR_TYPE> ReadCondition_type;
    typedef DDS_QueryCondition_T<DDS_TYPE, VENDOR_TYPE> QueryCondition_type;
    typedef DDS_ContentFilteredTopic_T<DDS_TYPE, VENDOR_TYPE> ContentFilteredTopic_type;
    typedef DDS_Subscriber_T<DDS_TYPE, VENDOR_TYPE> Subscriber_type;
    typedef DataReader_T<DDS_TYPE, VENDOR_TYPE> DataReader_type;
    public:
      /// Constructor
      explicit DataReader_T (DDSDataReader * dr = 0);

      /// Destructor
      virtual ~DataReader_T (void);

      void read_wo_instance (
        typename DDS_TYPE::dds_seq_type & data,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info,
        DDSQueryCondition * qc);

      void read_w_instance (
        typename DDS_TYPE::dds_seq_type & data,
        const ::DDS_InstanceHandle_t & lookup_hnd,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info);

      DDS_ReturnCode_t get (
        typename DDS_TYPE::dds_seq_type & data,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info,
        const DDS_Long & max_samples,
        DDSQueryCondition * qc);

      DDS_ReturnCode_t get (
        typename DDS_TYPE::dds_seq_type & data,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info,
        const DDS_Long & max_samples,
        DDSReadCondition * rd);

      DDS_ReturnCode_t take (
        typename DDS_TYPE::dds_seq_type & data,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info,
        const DDS_Long & max_samples,
        DDSQueryCondition * qc);

      DDS_InstanceHandle_t
      lookup_instance (const typename DDS_TYPE::value_type& an_instance);

      DDS_ReturnCode_t
      return_loan (
        typename DDS_TYPE::dds_seq_type & data,
        typename DDS_TYPE::sampleinfo_seq_type & sample_info);

      void delete_datareader (::DDS::Subscriber_ptr subscriber);

      void passivate (void);

      void
      create_datareader (::DDS::ContentFilteredTopic_ptr topic,
                         ::DDS::Subscriber_ptr subscriber,
                         const char * library_name,
                         const char * profile_name);

      void
      create_datareader (::DDS::Topic_ptr topic,
                         ::DDS::Subscriber_ptr subscriber,
                         const char * library_name,
                         const char * profile_name);

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
      set_listener (
        ::DDS::DataReaderListener_ptr a_listener,
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

      void set_impl (::DDS::DataReader_ptr dr);

      ::DDS::StatusMask get_mask (void);

      virtual DDSDataReader * get_impl (void);

    private:
      typename DDS_TYPE::datareader_type * impl_;

      ::DDS::StatusMask lst_mask_;

      typename DDS_TYPE::datareader_type * impl (void);

      void log_query_condition (DDSQueryCondition * qc);
    };
  }
}

#include "dds4ccm/impl/ndds/DataReader_T.cpp"

#endif /* DATAREADER_T_H_ */
