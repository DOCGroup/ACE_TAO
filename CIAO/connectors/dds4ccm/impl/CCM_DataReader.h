/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for DDS.
 */

#ifndef CCM_DATAREADER_T_H_
#define CCM_DATAREADER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/ContentFilteredTopic_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DataReader
      : public ::DDS::CCM_DataReader
    {
    public:
      /// Constructor
      CCM_DataReader (void);

      /// Destructor
      virtual ~CCM_DataReader (void);

      virtual ::DDS::ReturnCode_t enable (void) {return 0;}
      virtual ::DDS::StatusCondition_ptr get_statuscondition (void) {return 0;}
      virtual ::DDS::StatusMask get_status_changes (void) {return 0;}
      virtual ::DDS::InstanceHandle_t get_instance_handle (void) {return ::DDS::HANDLE_NIL;}
      virtual ::DDS::ReadCondition_ptr create_readcondition (::DDS::SampleStateMask /*sample_states*/,
      ::DDS::ViewStateMask /*view_states*/,
      ::DDS::InstanceStateMask /*instance_states*/)  {return 0;}
      virtual ::DDS::QueryCondition_ptr create_querycondition (::DDS::SampleStateMask /*sample_states*/,
      ::DDS::ViewStateMask /*view_states*/,
      ::DDS::InstanceStateMask /*instance_states*/,
      const char * /*query_expression*/,
      const ::DDS::StringSeq & /*query_parameters*/)  {return 0;}
      virtual ::DDS::ReturnCode_t delete_readcondition (::DDS::ReadCondition_ptr /*a_condition*/)  {return 0;}
      virtual ::DDS::ReturnCode_t delete_contained_entities (void)  {return 0;}
      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::DataReaderQos & /*qos*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_qos (::DDS::DataReaderQos & /*qos*/)  {return 0;}
      virtual ::DDS::ReturnCode_t set_listener (::DDS::DataReaderListener_ptr /*a_listener*/,
      ::DDS::StatusMask /*mask*/)  {return 0;}
      virtual ::DDS::DataReaderListener_ptr get_listener (void)  {return 0;}
      virtual ::DDS::TopicDescription_ptr get_topicdescription (void)  {return 0;}
      virtual ::DDS::Subscriber_ptr get_subscriber (void)  {return 0;}
      virtual ::DDS::ReturnCode_t get_sample_rejected_status (::DDS::SampleRejectedStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_liveliness_changed_status (::DDS::LivelinessChangedStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_requested_deadline_missed_status (::DDS::RequestedDeadlineMissedStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_requested_incompatible_qos_status (::DDS::RequestedIncompatibleQosStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_subscription_matched_status (::DDS::SubscriptionMatchedStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_sample_lost_status (::DDS::SampleLostStatus & /*status*/)  {return 0;}
      virtual ::DDS::ReturnCode_t wait_for_historical_data (const ::DDS::Duration_t & /*max_wait*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_matched_publications (::DDS::InstanceHandleSeq & /*publication_handles*/)  {return 0;}
      virtual ::DDS::ReturnCode_t get_matched_publication_data (::DDS::PublicationBuiltinTopicData & /*publication_data*/,
      const ::DDS::InstanceHandle_t & /*publication_handle*/)  {return 0;}


      void set_impl (::DDS::DataReader_ptr dr);

      ::DDS::DataReader_ptr
      get_impl (void);

    private:
      ::DDS::DataReader_var impl_;
      ::DDS::DataReader_ptr impl (void);
    };
  }
}

#endif /* CCM_DATAREADER_T_H_ */
