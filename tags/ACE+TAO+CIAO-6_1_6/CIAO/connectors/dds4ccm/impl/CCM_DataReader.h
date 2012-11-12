/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for DDS.
 */

#ifndef CCM_DATAREADER_T_H_
#define CCM_DATAREADER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DataReader
      : public virtual ::DDS::CCM_DataReader,
        public virtual LocalObject
    {
    public:
      /// Constructor
      CCM_DataReader (void);

      /// Destructor
      virtual ~CCM_DataReader (void);

      virtual ::DDS::ReturnCode_t
      enable (void);

      virtual ::DDS::StatusCondition_ptr
      get_statuscondition (void);

      virtual ::DDS::StatusMask
      get_status_changes (void);

      virtual ::DDS::InstanceHandle_t
      get_instance_handle (void);

      virtual ::DDS::ReadCondition_ptr
      create_readcondition (::DDS::SampleStateMask ,
                            ::DDS::ViewStateMask ,
                            ::DDS::InstanceStateMask);
      virtual ::DDS::QueryCondition_ptr
      create_querycondition (::DDS::SampleStateMask ,
                             ::DDS::ViewStateMask ,
                             ::DDS::InstanceStateMask ,
                             const char * ,
                             const ::DDS::StringSeq & );

      virtual ::DDS::ReturnCode_t
      delete_readcondition (::DDS::ReadCondition_ptr );

      virtual ::DDS::ReturnCode_t delete_contained_entities (void);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::DataReaderQos & );

      virtual ::DDS::ReturnCode_t get_qos (::DDS::DataReaderQos & );

      virtual ::DDS::ReturnCode_t
      set_listener (::DDS::DataReaderListener_ptr,
                    ::DDS::StatusMask );

      virtual ::DDS::DataReaderListener_ptr
      get_listener (void);

      virtual ::DDS::TopicDescription_ptr
      get_topicdescription (void);

      virtual ::DDS::Subscriber_ptr
      get_subscriber (void);

      virtual ::DDS::ReturnCode_t
      get_sample_rejected_status (::DDS::SampleRejectedStatus & );

      virtual ::DDS::ReturnCode_t
      get_liveliness_changed_status (::DDS::LivelinessChangedStatus & );

      virtual ::DDS::ReturnCode_t
      get_requested_deadline_missed_status (::DDS::RequestedDeadlineMissedStatus & );

      virtual ::DDS::ReturnCode_t
      get_requested_incompatible_qos_status (::DDS::RequestedIncompatibleQosStatus &  );

      virtual ::DDS::ReturnCode_t
      get_subscription_matched_status (::DDS::SubscriptionMatchedStatus & );

      virtual ::DDS::ReturnCode_t
      get_sample_lost_status (::DDS::SampleLostStatus & );

      virtual ::DDS::ReturnCode_t
      wait_for_historical_data (const ::DDS::Duration_t & );

      virtual ::DDS::ReturnCode_t
      get_matched_publications (::DDS::InstanceHandleSeq & );

#if !defined (DDS_HAS_MINIMUM_BIT)
      virtual ::DDS::ReturnCode_t
      get_matched_publication_data (::DDS::PublicationBuiltinTopicData & ,
                                    DDS_INSTANCE_HANDLE_T_IN );
#endif

      void set_dds_entity (::DDS::DataReader_ptr dr);

      ::DDS::DataReader_ptr
      get_dds_entity (void);

    private:
      ::DDS::DataReader_var dds_entity_;
      ::DDS::DataReader_ptr dds_entity (void);
    };
  }
}

#endif /* CCM_DATAREADER_T_H_ */
