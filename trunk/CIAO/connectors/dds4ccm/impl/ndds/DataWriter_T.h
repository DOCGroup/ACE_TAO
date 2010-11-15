/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAWRITER_T_H_
#define DATAWRITER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/ndds/DataWriter.h"

namespace CIAO
{
  namespace NDDS
  {
    template <typename DDS_TYPE>
    class DataWriter_T
      : public virtual DDS_DataWriter_Base,
        public virtual DDS_TYPE::typed_writer_type
    {
    public:
      explicit DataWriter_T (::DDSDataWriter * dw = 0,
      ::DDS::DomainParticipant_ptr dp = ::DDS::DomainParticipant::_nil ()); // @todo);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::DataWriterQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (::DDS::DataWriterQos & qos);

      virtual ::DDS::ReturnCode_t set_listener (
        ::DDS::DataWriterListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::DataWriterListener_ptr get_listener (void);

      virtual ::DDS::Topic_ptr get_topic (void);

      virtual ::DDS::Publisher_ptr get_publisher (void);

      virtual ::DDS::ReturnCode_t wait_for_acknowledgments (
        const ::DDS::Duration_t & max_wait);

      virtual ::DDS::ReturnCode_t get_liveliness_lost_status (
        ::DDS::LivelinessLostStatus & status);

      virtual ::DDS::ReturnCode_t get_offered_deadline_missed_status (
        ::DDS::OfferedDeadlineMissedStatus & status);

      virtual ::DDS::ReturnCode_t get_offered_incompatible_qos_status (
        ::DDS::OfferedIncompatibleQosStatus & status);

      virtual ::DDS::ReturnCode_t get_publication_matched_status (
        ::DDS::PublicationMatchedStatus & status);

      virtual ::DDS::ReturnCode_t assert_liveliness (void);

      virtual ::DDS::ReturnCode_t get_matched_subscriptions (
        ::DDS::InstanceHandleSeq & subscription_handles);

      virtual ::DDS::ReturnCode_t get_matched_subscription_data (
        ::DDS::SubscriptionBuiltinTopicData & subscription_data,
        DDS_INSTANCE_HANDLE_T_IN subscription_handle);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual DDS_INSTANCE_HANDLE_T_RETN get_instance_handle (void);

      virtual ::DDS::InstanceHandle_t register_instance (
        const typename DDS_TYPE::value_type& instance_data);

      virtual ::DDS::InstanceHandle_t register_instance_w_timestamp (
        const typename  DDS_TYPE::value_type & instance_data,
        const ::DDS::Time_t & source_timestamp);

      virtual ::DDS::ReturnCode_t unregister_instance (
        const typename DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle);

      virtual ::DDS::ReturnCode_t unregister_instance_w_timestamp (
        const typename  DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle,
        const ::DDS::Time_t & source_timestamp);

      virtual ::DDS::ReturnCode_t write (
        const typename DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle);

      virtual ::DDS::ReturnCode_t write_w_timestamp (
        const typename DDS_TYPE::value_type & instance_data,
        const ::DDS::InstanceHandle_t & handle,
        const ::DDS::Time_t & source_timestamp);

      virtual ::DDS::ReturnCode_t dispose (
        const typename DDS_TYPE::value_type& instance_data,
        const ::DDS::InstanceHandle_t & instance_handle);

      virtual ::DDS::ReturnCode_t dispose_w_timestamp (
        const typename DDS_TYPE::value_type& instance_data,
        const ::DDS::InstanceHandle_t & instance_handle,
        const ::DDS::Time_t & source_timestamp);

      virtual ::DDS::ReturnCode_t get_key_value (
        typename DDS_TYPE::value_type & key_holder,
        const ::DDS::InstanceHandle_t & handle);

      virtual ::DDS::InstanceHandle_t lookup_instance (
        const typename DDS_TYPE::value_type& instance_data);

      virtual ::DDSDataWriter * get_rti_entity (void);

      // @todo, this should be private to my idea
      virtual void set_rti_entity (::DDSDataWriter * dw);

    private:
      typename DDS_TYPE::datawriter_type * rti_entity_;
      ::DDS::DomainParticipant_var dp_;

      typename DDS_TYPE::datawriter_type * rti_entity (void);
    };
  }
}

#include "dds4ccm/impl/ndds/DataWriter_T.cpp"

#endif /* DATAWRITER_T_H_ */
