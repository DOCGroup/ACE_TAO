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

#include "dds4ccm/impl/ndds/ContentFilteredTopic_T.h"

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

      virtual ::DDS::ReturnCode_t enable (void) {return 0;}
      virtual ::DDS::StatusCondition_ptr get_statuscondition (void) {return 0;}
      virtual ::DDS::StatusMask get_status_changes (void) {return 0;}
      virtual ::DDS::InstanceHandle_t get_instance_handle (void) {return ::DDS::HANDLE_NIL;}
      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::DataWriterQos & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_qos (::DDS::DataWriterQos & ) {return 0;}
      virtual ::DDS::ReturnCode_t set_listener (::DDS::DataWriterListener_ptr ,
      ::DDS::StatusMask ) {return 0;}
      virtual ::DDS::DataWriterListener_ptr get_listener (void) {return 0;}
      virtual ::DDS::Topic_ptr get_topic (void) {return 0;}
      virtual ::DDS::Publisher_ptr get_publisher (void) {return 0;}
      virtual ::DDS::ReturnCode_t wait_for_acknowledgments (const ::DDS::Duration_t & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_liveliness_lost_status (::DDS::LivelinessLostStatus & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_publication_matched_status (::DDS::PublicationMatchedStatus & ) {return 0;}
      virtual ::DDS::ReturnCode_t assert_liveliness (void) {return 0;}
      virtual ::DDS::ReturnCode_t get_matched_subscriptions (::DDS::InstanceHandleSeq & ) {return 0;}
      virtual ::DDS::ReturnCode_t get_matched_subscription_data (::DDS::SubscriptionBuiltinTopicData & ,
      const ::DDS::InstanceHandle_t & ) {return 0;}

      void set_impl (::DDS::DataWriter_ptr dr);

      ::DDS::DataWriter_ptr
      get_impl (void);

    private:
      ::DDS::DataWriter_var impl_;
      ::DDS::DataWriter_ptr impl (void);
    };
  }
}

#endif /* CCM_DATAREADER_T_H_ */
