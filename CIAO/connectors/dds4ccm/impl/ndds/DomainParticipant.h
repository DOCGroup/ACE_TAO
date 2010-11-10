/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DOMAINPARTICIPANT_T_H_
#define DOMAINPARTICIPANT_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"
#include "tao/LocalObject.h"

#include <map>

class TypeFactory
{
  public:
    virtual DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp) = 0;
    virtual DDS::DataReader_ptr create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp) = 0;
};

template <typename DDS_TYPE>
class DDSTypeFactory : public TypeFactory
{
  public:
    DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp)
    {
      typedef CIAO::NDDS::DataWriter_T<DDS_TYPE> DataWriter_type;
      return new DataWriter_type (dw, dp);
    }
    DDS::DataReader_ptr create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp)
    {
      typedef CIAO::NDDS::DataReader_T<DDS_TYPE> DataReader_type;
      return new DataReader_type (dr, dp);
    }
};

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DomainParticipant_i :
      public virtual ::DDS::DomainParticipant,
      public virtual ::CORBA::LocalObject
    {
    public:
      /* @todo, cleanup this */
      void register_type (const char* type, TypeFactory*);
      ::DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw);
      ::DDS::DataReader_ptr create_datareader (DDSDataReader* dr);

      /// Constructor
      DDS_DomainParticipant_i (DDSDomainParticipant * dp);

      /// Destructor
      virtual ~DDS_DomainParticipant_i (void);

      virtual ::DDS::Publisher_ptr create_publisher (
        const ::DDS::PublisherQos & qos,
        ::DDS::PublisherListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::Publisher_ptr create_publisher_with_profile (
        const char* library_name,
        const char *profile_name,
        ::DDS::PublisherListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::ReturnCode_t delete_publisher (::DDS::Publisher_ptr p);

      virtual ::DDS::Subscriber_ptr create_subscriber (
        const ::DDS::SubscriberQos & qos,
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::Subscriber_ptr create_subscriber_with_profile(
        const char* library_name,
        const char *profile_name,
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::ReturnCode_t delete_subscriber (::DDS::Subscriber_ptr s);

      virtual ::DDS::Subscriber_ptr get_builtin_subscriber (void);

      virtual ::DDS::Topic_ptr create_topic (
        const char * impl_name,
        const char * type_name,
        const ::DDS::TopicQos & qos,
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::Topic_ptr create_topic_with_profile (
        const char * impl_name,
        const char * type_name,
        const char* library_name,
        const char *profile_name,
        ::DDS::TopicListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::ReturnCode_t delete_topic (::DDS::Topic_ptr a_topic);

      virtual ::DDS::Topic_ptr find_topic (
        const char * impl_name,
        const ::DDS::Duration_t & timeout);

      virtual ::DDS::TopicDescription_ptr lookup_topicdescription (
        const char * name);

      virtual ::DDS::ContentFilteredTopic_ptr create_contentfilteredtopic (
        const char * name,
        ::DDS::Topic_ptr related_topic,
        const char * filter_expression,
        const ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::ReturnCode_t delete_contentfilteredtopic (
        ::DDS::ContentFilteredTopic_ptr a_contentfilteredtopic);

      virtual ::DDS::MultiTopic_ptr create_multitopic (
        const char * name,
        const char * type_name,
        const char * subscription_expression,
        const ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::ReturnCode_t delete_multitopic (
        ::DDS::MultiTopic_ptr a_multitopic);

      virtual ::DDS::ReturnCode_t delete_contained_entities (void);

      virtual ::DDS::ReturnCode_t set_qos (
        const ::DDS::DomainParticipantQos & qos);

      virtual ::DDS::ReturnCode_t get_qos (
        ::DDS::DomainParticipantQos & qos);

      virtual ::DDS::ReturnCode_t set_listener (
        ::DDS::DomainParticipantListener_ptr a_listener,
        ::DDS::StatusMask mask);

      virtual ::DDS::DomainParticipantListener_ptr get_listener (void);

      virtual ::DDS::ReturnCode_t ignore_participant (
        DDS_INSTANCE_HANDLE_T_IN handle);

      virtual ::DDS::ReturnCode_t ignore_topic (
        DDS_INSTANCE_HANDLE_T_IN handle);

      virtual ::DDS::ReturnCode_t ignore_publication (
        DDS_INSTANCE_HANDLE_T_IN handle);

      virtual ::DDS::ReturnCode_t ignore_subscription (
        DDS_INSTANCE_HANDLE_T_IN handle);

      virtual ::DDS::DomainId_t get_domain_id (void);

      virtual ::DDS::ReturnCode_t assert_liveliness (void);

      virtual ::DDS::ReturnCode_t set_default_publisher_qos (
        const ::DDS::PublisherQos & qos);

      virtual ::DDS::ReturnCode_t get_default_publisher_qos (
        ::DDS::PublisherQos & qos);

      virtual ::DDS::ReturnCode_t set_default_subscriber_qos (
        const ::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t get_default_subscriber_qos (
        ::DDS::SubscriberQos & qos);

      virtual ::DDS::ReturnCode_t set_default_topic_qos (
        const ::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t get_default_topic_qos (
        ::DDS::TopicQos & qos);

      virtual ::DDS::ReturnCode_t get_discovered_participants (
        ::DDS::InstanceHandleSeq & impl_handles);

      virtual ::DDS::ReturnCode_t get_discovered_participant_data (
        ::DDS::ParticipantBuiltinTopicData & impl_data,
        DDS_INSTANCE_HANDLE_T_IN impl_handle);

      virtual ::DDS::ReturnCode_t get_discovered_topics (
        ::DDS::InstanceHandleSeq & impl_handles);

      virtual ::DDS::ReturnCode_t get_discovered_topic_data (
        ::DDS::TopicBuiltinTopicData & impl_data,
        DDS_INSTANCE_HANDLE_T_IN impl_handle);

      virtual ::CORBA::Boolean contains_entity (
        DDS_INSTANCE_HANDLE_T_IN a_handle);

      virtual ::DDS::ReturnCode_t get_current_time (
        ::DDS::Time_t & current_time);

      virtual ::DDS::ReturnCode_t enable (void);

      virtual ::DDS::StatusCondition_ptr get_statuscondition (void);

      virtual ::DDS::StatusMask get_status_changes (void);

      virtual DDS_INSTANCE_HANDLE_T_RETN get_instance_handle (void);

      ::DDSDomainParticipant * get_rti_entity (void);

      void set_rti_entity (::DDSDomainParticipant * dp);

    protected:
      typedef std::map <ACE_CString, TypeFactory*> typefactories;
      typefactories type_factories;
      TypeFactory* factory_;
      ::DDSDomainParticipant *rti_entity_;
      ::DDSDomainParticipant * rti_entity (void);
    };
  }
}

#endif /* DOMAINPARTICIPANT_T_H_ */
