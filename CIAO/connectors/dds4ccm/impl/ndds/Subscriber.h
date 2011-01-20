/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SUBSCRIBER_T_H_
#define SUBSCRIBER_T_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"
#include "tao/LocalObject.h"

class DDSSubscriber;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_Subscriber_i :
      public virtual ::DDS::Subscriber,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_Subscriber_i (::DDSSubscriber * sub,
                      ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_Subscriber_i (void);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual ::DDS::InstanceHandle_t get_instance_handle (void);

      virtual ::DDS::DataReader_ptr create_datareader (
        ::DDS::TopicDescription_ptr a_topic,
        const ::DDS::DataReaderQos & qos,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::DataReader_ptr
      create_datareader_with_profile (
        ::DDS::TopicDescription_ptr a_topic,
        const char * qos_profile,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::ReturnCode_t
      delete_datareader (::DDS::DataReader_ptr a_datareader);

      virtual ::DDS::ReturnCode_t delete_contained_entities (void);

      virtual ::DDS::DataReader_ptr lookup_datareader (const char * impl_name);

      virtual ::DDS::ReturnCode_t
      get_datareaders (
        ::DDS::DataReaderSeq & readers,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t notify_datareaders (void);

      virtual ::DDS::ReturnCode_t set_qos (const ::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t
      set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::SubscriberListener_ptr get_listener (void);

      virtual ::DDS::ReturnCode_t begin_access (void);

      virtual ::DDS::ReturnCode_t end_access (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      virtual ::DDS::ReturnCode_t
      set_default_datareader_qos (
        const ::DDS::DataReaderQos & qos);

      virtual ::DDS::ReturnCode_t
      get_default_datareader_qos (
        ::DDS::DataReaderQos & qos);

      virtual ::DDS::ReturnCode_t
      copy_from_topic_qos (
        ::DDS::DataReaderQos & a_datareader_qos,
        const ::DDS::TopicQos & a_impl_qos);

      ::DDSSubscriber * get_rti_entity (void);

      void set_rti_entity (::DDSSubscriber * sub);

    private:
      DDSSubscriber *rti_entity_;
      ::DDS::DomainParticipant_var dp_;

      DDSSubscriber *rti_entity (void);

      DDSDataReader *
      create_datareader (
        DDSTopic * topic,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask,
         const ::DDS::DataReaderQos & qos);

      DDSDataReader *
      create_datareader (
        DDSContentFilteredTopic * topic,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask,
         const ::DDS::DataReaderQos & qos);

      DDSDataReader *
      create_datareader_with_profile (
        DDSContentFilteredTopic * topic,
        const char * qos_profile,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask);

      DDSDataReader *
      create_datareader_with_profile (
        DDSTopic * topic,
        const char * qos_profile,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask);
    };
  }
}

#endif /* SUBSCRIBER_T_H_ */
