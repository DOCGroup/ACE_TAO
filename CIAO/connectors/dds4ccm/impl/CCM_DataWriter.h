/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for DDS.
 */

#ifndef CCM_DATAWRITER_T_H_
#define CCM_DATAWRITER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DataWriter
      : public ::DDS::CCM_DataWriter
    {
    public:
      /// Constructor
      CCM_DataWriter (void);

      /// Destructor
      virtual ~CCM_DataWriter (void);

      virtual ::DDS::ReturnCode_t
      enable (void);

      virtual ::DDS::StatusCondition_ptr
      get_statuscondition (void);

      virtual ::DDS::StatusMask
      get_status_changes (void);

      virtual ::DDS::InstanceHandle_t
      get_instance_handle (void);

      virtual ::DDS::ReturnCode_t
      set_qos (const ::DDS::DataWriterQos & );

      virtual ::DDS::ReturnCode_t
      get_qos (::DDS::DataWriterQos & );

      virtual ::DDS::ReturnCode_t
      set_listener (::DDS::DataWriterListener_ptr ,
                    ::DDS::StatusMask );

      virtual ::DDS::DataWriterListener_ptr
      get_listener (void);

      virtual ::DDS::Topic_ptr
      get_topic (void);

      virtual ::DDS::Publisher_ptr
      get_publisher (void);

      virtual ::DDS::ReturnCode_t
      wait_for_acknowledgments (const ::DDS::Duration_t & );

      virtual ::DDS::ReturnCode_t
      get_liveliness_lost_status (::DDS::LivelinessLostStatus & );

      virtual ::DDS::ReturnCode_t
      get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & );

      virtual ::DDS::ReturnCode_t
      get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & );

      virtual ::DDS::ReturnCode_t
      get_publication_matched_status (::DDS::PublicationMatchedStatus & );

      virtual ::DDS::ReturnCode_t
      assert_liveliness (void);

      virtual ::DDS::ReturnCode_t
      get_matched_subscriptions (::DDS::InstanceHandleSeq & );

      virtual ::DDS::ReturnCode_t
      get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & ,
                                     const ::DDS::InstanceHandle_t & );

      void set_dds_entity (::DDS::DataWriter_ptr dw);

      ::DDS::DataWriter_ptr
      get_dds_entity (void);

    private:
      ::DDS::DataWriter_var impl_;
      ::DDS::DataWriter_ptr impl (void);
    };
  }
}

#endif /* CCM_DATAREADER_T_H_ */
