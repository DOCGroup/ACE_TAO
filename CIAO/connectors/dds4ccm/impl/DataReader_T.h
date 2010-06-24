/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Common dataReader class for the Getter, Reader,
 * and, Listen port
 *
 */
#ifndef DDS_DATAREADER_T_H
#define DDS_DATAREADER_T_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::InstanceHandle_t DDS_InstanceHandle_t;
typedef ::DDS::SampleInfoSeq DDS_SampleInfoSeq;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_ReadCondition_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_QueryCondition_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_ContentFilteredTopic_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_DataReaderListener_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReader_T
      : public virtual ::DDS::CCM_DataReader,
        public virtual ::CORBA::LocalObject
    {
    typedef ::CIAO::DDS4CCM::CCM_DDS_DataReaderListener_T<DDS_TYPE, CCM_TYPE>
      DataReaderListener_type;
    typedef CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE> Topic_type;
    typedef CCM_DDS_ReadCondition_T<DDS_TYPE, CCM_TYPE> ReadCondition_type;
    typedef CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE> QueryCondition_type;
    typedef CCM_DDS_ContentFilteredTopic_T<DDS_TYPE, CCM_TYPE> ContentFilteredTopic_type;
    typedef CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE> Subscriber_type;
    typedef DataReader_T<DDS_TYPE, CCM_TYPE> DataReader_type;
    public:
      /// Constructor
      DataReader_T (DDSDataReader * dr);

      /// Destructor
      virtual ~DataReader_T (void);

      void read_w_instance (
        typename DDS_TYPE::dds_seq_type & data,
        const ::DDS_InstanceHandle_t & lookup_hnd,
        ::DDS_SampleInfoSeq & sample_info);

      void read_wo_instance (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info);

      DDS_ReturnCode_t read_w_condition (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info,
        ::DDS_Long max_samples);

      DDS_ReturnCode_t take (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info,
        ::DDS_Long max_samples);

      DDS_InstanceHandle_t
      lookup_instance (const typename DDS_TYPE::value_type& an_instance);

      DDS_ReturnCode_t
      return_loan (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info);

      void delete_datareader (::DDS::Subscriber_ptr subscriber);

      ::CCM_DDS::QueryFilter *query (void);

      void query (const ::CCM_DDS::QueryFilter & filter);

      void
      set_filter (const ::CCM_DDS::QueryFilter & filter,
                  DDSQueryCondition * qc);

      // Getter related methods.
      void remove_condition (DDSQueryCondition * qc, const char * type);

      void remove_conditions (void);

      DDSReadCondition *get_readcondition (void);

      void create_readcondition (void);

      DDSQueryCondition *get_querycondition (void);

      void attach_querycondition (void);

      bool
      wait (DDSConditionSeq & active_conditions, DDS_Duration_t & time_out);

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

      typename DDS_TYPE::data_reader * get_impl (void);

      void set_impl (::DDS::DataReader_ptr dr);

      ::DDS::StatusMask get_mask (void);

    private:
      typename DDS_TYPE::data_reader * impl_;

      ::DDSReadCondition * rd_condition_;

      ::DDSWaitSet * ws_;

      // Different QueryConditions since the sample mask
      // differs for all entities.
      DDSQueryCondition * qc_reader_;
      DDSQueryCondition * qc_getter_;
      DDSQueryCondition * qc_listener_;

      ::DDS::StatusMask lst_mask_;

      typename DDS_TYPE::data_reader * impl (void);
    };
  }
}

#include "dds4ccm/impl/DataReader_T.cpp"

#endif /* DDS_DATAREADER_T_H */
