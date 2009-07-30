/**
 * @file Participant.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Simple abstraction for a RTI DDS participant
 */

#ifndef CIAO_NDDS_PARTICIPANT_H
#define CIAO_NDDS_PARTICIPANT_H

#include "tao/LocalObject.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DomainParticipant_i :
        public virtual DDS::CCM_DomainParticipant
      {
      public:
        // Constructor
        RTI_DomainParticipant_i (DDSDomainParticipant *p);

        // Destructor
        virtual ~RTI_DomainParticipant_i (void);

        virtual
          ::DDS::Publisher_ptr create_publisher (const ::DDS::PublisherQos & qos,
                                                 ::DDS::PublisherListener_ptr a_listener,
                                                 ::DDS::StatusMask mask);

        virtual
          ::DDS::ReturnCode_t delete_publisher (::DDS::Publisher_ptr p);

        virtual
          ::DDS::Subscriber_ptr create_subscriber (const ::DDS::SubscriberQos & qos,
                                                   ::DDS::SubscriberListener_ptr a_listener,
                                                   ::DDS::StatusMask mask);

        virtual
          ::DDS::ReturnCode_t delete_subscriber (::DDS::Subscriber_ptr s);

        virtual
          ::DDS::Subscriber_ptr get_builtin_subscriber (void);

        virtual
          ::DDS::Topic_ptr create_topic (const char * topic_name,
                                         const char * type_name,
                                         const ::DDS::TopicQos & qos,
                                         ::DDS::TopicListener_ptr a_listener,
                                         ::DDS::StatusMask mask);

        virtual
          ::DDS::ReturnCode_t delete_topic (::DDS::Topic_ptr a_topic);

        virtual
          ::DDS::Topic_ptr find_topic (const char * topic_name,
                                       const ::DDS::Duration_t & timeout);

        virtual
          ::DDS::TopicDescription_ptr lookup_topicdescription (const char * name);

        virtual
          ::DDS::ContentFilteredTopic_ptr create_contentfilteredtopic (const char * name,
                                                                       ::DDS::Topic_ptr related_topic,
                                                                       const char * filter_expression,
                                                                       const ::DDS::StringSeq & expression_parameters);

        virtual
          ::DDS::ReturnCode_t delete_contentfilteredtopic (::DDS::ContentFilteredTopic_ptr a_contentfilteredtopic);

        virtual
          ::DDS::MultiTopic_ptr create_multitopic (const char * name,
                                                   const char * type_name,
                                                   const char * subscription_expression,
                                                   const ::DDS::StringSeq & expression_parameters);

        virtual
          ::DDS::ReturnCode_t delete_multitopic (::DDS::MultiTopic_ptr a_multitopic);

        virtual
          ::DDS::ReturnCode_t delete_contained_entities (void);

        virtual
          ::DDS::ReturnCode_t set_qos (const ::DDS::DomainParticipantQos & qos);

        virtual
          ::DDS::ReturnCode_t get_qos (::DDS::DomainParticipantQos & qos);

        virtual
          ::DDS::ReturnCode_t set_listener (::DDS::DomainParticipantListener_ptr a_listener,
                                            ::DDS::StatusMask mask);

        virtual
          ::DDS::DomainParticipantListener_ptr get_listener (void);

        virtual
          ::DDS::ReturnCode_t ignore_participant (const ::DDS::InstanceHandle_t & handle);

        virtual
          ::DDS::ReturnCode_t ignore_topic (const ::DDS::InstanceHandle_t & handle);

        virtual
          ::DDS::ReturnCode_t ignore_publication (const ::DDS::InstanceHandle_t & handle);

        virtual
          ::DDS::ReturnCode_t ignore_subscription (const ::DDS::InstanceHandle_t & handle);

        virtual
          ::DDS::DomainId_t get_domain_id (void);

        virtual
          ::DDS::ReturnCode_t assert_liveliness (void);

        virtual
          ::DDS::ReturnCode_t set_default_publisher_qos (const ::DDS::PublisherQos & qos);

        virtual
          ::DDS::ReturnCode_t get_default_publisher_qos (::DDS::PublisherQos & qos);

        virtual
          ::DDS::ReturnCode_t set_default_subscriber_qos (const ::DDS::SubscriberQos & qos);

        virtual
          ::DDS::ReturnCode_t get_default_subscriber_qos (::DDS::SubscriberQos & qos);

        virtual
          ::DDS::ReturnCode_t set_default_topic_qos (const ::DDS::TopicQos & qos);

        virtual
          ::DDS::ReturnCode_t get_default_topic_qos (::DDS::TopicQos & qos);

        virtual
          ::DDS::ReturnCode_t get_discovered_participants (::DDS::InstanceHandleSeq & participant_handles);

        virtual
          ::DDS::ReturnCode_t get_discovered_participant_data (::DDS::ParticipantBuiltinTopicData & participant_data,
                                                               const ::DDS::InstanceHandle_t & participant_handle);

        virtual
          ::DDS::ReturnCode_t get_discovered_topics (::DDS::InstanceHandleSeq & topic_handles);

        virtual
          ::DDS::ReturnCode_t get_discovered_topic_data (::DDS::TopicBuiltinTopicData & topic_data,
                                                         const ::DDS::InstanceHandle_t & topic_handle);

        virtual
          ::CORBA::Boolean contains_entity (const ::DDS::InstanceHandle_t & a_handle);

        virtual
          ::DDS::ReturnCode_t get_current_time (::DDS::Time_t & current_time);

        virtual
          ::DDS::ReturnCode_t enable (void);

        virtual
          ::DDS::StatusCondition_ptr get_statuscondition (void);

        virtual
          ::DDS::StatusMask get_status_changes (void);

        virtual
          ::DDS::InstanceHandle_t get_instance_handle (void);

        DDSDomainParticipant * get_participant (void);

      protected:
        DDSDomainParticipant *participant_;
      };
    }
  }
}

#endif
