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

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/InstanceHandle_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/DataWriter.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DataWriter DDSDataWriter;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_DataWriterListener_T;

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_Publisher_T;

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_Topic_T;

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_DataWriter_T
      : public virtual CCM_DDS_DataWriter_Base
    {
    typedef CCM_DDS_DataWriterListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DataWriterListener_type;
    typedef CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> Publisher_type;
    typedef CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> Topic_type;

    public:
      explicit CCM_DDS_DataWriter_T (DDSDataWriter * dw = 0);

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
    };
  }
}

#include "dds4ccm/impl/DataWriter_T.cpp"

#endif /* DATAWRITER_T_H_ */
