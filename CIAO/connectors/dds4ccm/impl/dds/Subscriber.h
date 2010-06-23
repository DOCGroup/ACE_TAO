/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SUBSCRIBER_H
#define DDS4CCM_SUBSCRIBER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "tao/LocalObject.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Subscriber DDSSubscriber;
typedef ::DDS::DataReaderListener DDSDataReaderListener;
typedef ::DDS::DataReader DDSDataReader;
typedef ::DDS::ContentFilteredTopic DDSContentFilteredTopic;
typedef ::DDS::Topic DDSTopic;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_DataReaderListener_i;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReader_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_Topic_i;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_ContentFilteredTopic_i;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_SubscriberListener_i;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_DomainParticipant_i;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_Subscriber_i :
      public virtual ::DDS::CCM_Subscriber,
      public virtual ::CORBA::LocalObject
    {

    typedef CCM_DDS_DataReaderListener_i<DDS_TYPE, CCM_TYPE> DataReaderListener_type;
    typedef CCM_DDS_Topic_i<DDS_TYPE, CCM_TYPE> Topic_type;
    typedef CCM_DDS_ContentFilteredTopic_i<DDS_TYPE, CCM_TYPE> ContentFilteredTopic_type;
    typedef DataReader_T<DDS_TYPE, CCM_TYPE> DataReader_type;
    typedef CCM_DDS_SubscriberListener_i<DDS_TYPE, CCM_TYPE> SubscriberListener_type;
    typedef CCM_DDS_DomainParticipant_i<DDS_TYPE, CCM_TYPE> DomainParticipant_type;

    public:
      /// Constructor
      CCM_DDS_Subscriber_i (DDSSubscriber * sub);

      /// Destructor
      virtual ~CCM_DDS_Subscriber_i (void);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual ::DDS::InstanceHandle_t get_instance_handle (void);

      virtual ::DDS::DataReader_ptr create_datareader (
        ::DDS::TopicDescription_ptr a_topic,
        const ::DDS::DataReaderQos & qos,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask);

#if (CIAO_DDS4CCM_NDDS==1)
      virtual ::DDS::DataReader_ptr
      create_datareader_with_profile (
        ::DDS::TopicDescription_ptr a_topic,
        const char* library_name,
        const char *profile_name,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask);
#endif

      virtual ::DDS::ReturnCode_t
      delete_datareader (
        ::DDS::DataReader_ptr a_datareader);

      virtual ::DDS::ReturnCode_t delete_contained_entities (void);

      virtual ::DDS::DataReader_ptr lookup_datareader (
        const char * impl_name);

      virtual ::DDS::ReturnCode_t
      get_datareaders (
        ::DDS::DataReaderSeq & readers,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states);

      virtual ::DDS::ReturnCode_t
      notify_datareaders (void);

      virtual ::DDS::ReturnCode_t
      set_qos (
        const ::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t
      get_qos (
        ::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t
      set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::SubscriberListener_ptr
      get_listener (void);

      virtual ::DDS::ReturnCode_t
      begin_access (void);

      virtual ::DDS::ReturnCode_t
      end_access (void);

      virtual ::DDS::DomainParticipant_ptr
      get_participant (void);

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

      DDSSubscriber * get_impl (void);

      void set_impl (DDSSubscriber * sub);

    private:
      DDSSubscriber *impl_;

      DDSSubscriber *impl (void);

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

#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataReader *
      create_datareader_with_profile (
        DDSContentFilteredTopic * topic,
        const char* library_name,
        const char *profile_name,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask);
#endif

#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataReader *
      create_datareader_with_profile (
        DDSTopic * topic,
        const char* library_name,
        const char *profile_name,
        DDSDataReaderListener * ccm_dds_drl,
        ::DDS::StatusMask mask);
#endif
    };
  }
}

#include "dds4ccm/impl/dds/Subscriber.cpp"

#endif
