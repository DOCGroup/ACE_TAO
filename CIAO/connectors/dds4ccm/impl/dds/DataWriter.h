/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAWRITER_H
#define DDS4CCM_DATAWRITER_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/dds/dds4ccm_ndds_impl_export.h"
#include "dds4ccm/impl/dds/ndds/InstanceHandle_t.h"
#include "tao/LocalObject.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DataWriter DDSDataWriter;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_IMPL_Export CCM_DDS_DataWriter_i
      : public virtual ::DDS::CCM_DataWriter,
        public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_DataWriter_i (DDSDataWriter * dw);

      /// Destructor
      virtual ~CCM_DDS_DataWriter_i (void);

      virtual
        ::DDS::ReturnCode_t set_qos (const ::DDS::DataWriterQos & qos);

      virtual
        ::DDS::ReturnCode_t get_qos (::DDS::DataWriterQos & qos);

      virtual
        ::DDS::ReturnCode_t set_listener (::DDS::DataWriterListener_ptr a_listener,
                                          ::DDS::StatusMask mask);

      virtual
        ::DDS::DataWriterListener_ptr get_listener (void);

      virtual
        ::DDS::Topic_ptr get_topic (void);

      virtual
        ::DDS::Publisher_ptr get_publisher (void);

      virtual
        ::DDS::ReturnCode_t wait_for_acknowledgments (const ::DDS::Duration_t & max_wait);

      virtual
        ::DDS::ReturnCode_t get_liveliness_lost_status (::DDS::LivelinessLostStatus & status);

      virtual
        ::DDS::ReturnCode_t get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & status);

      virtual
        ::DDS::ReturnCode_t get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & status);

      virtual
        ::DDS::ReturnCode_t get_publication_matched_status (::DDS::PublicationMatchedStatus & status);

      virtual
        ::DDS::ReturnCode_t assert_liveliness (void);

      virtual
        ::DDS::ReturnCode_t get_matched_subscriptions (::DDS::InstanceHandleSeq & subscription_handles);

      virtual
        ::DDS::ReturnCode_t get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & subscription_data,
                                                           DDS_INSTANCE_HANDLE_T_IN subscription_handle);

      virtual
        ::DDS::ReturnCode_t enable (void);

      virtual
        ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual
        ::DDS::StatusMask get_status_changes (void);

      virtual
        DDS_INSTANCE_HANDLE_T_RETN get_instance_handle (void);

      DDSDataWriter * get_impl (void);

      void set_impl (DDSDataWriter * dw);

    private:
      DDSDataWriter * impl_;

      DDSDataWriter * impl (void);
    };
  }
}

#endif
