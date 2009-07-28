// $Id$

#include "Participant.h"
#include "Subscriber.h"
#include "Publisher.h"
#include "Topic.h"
#include "Utils.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DomainParticipant_i::RTI_DomainParticipant_i (DDSDomainParticipant *part)
        : participant_ (part)
      {
        CIAO_TRACE ("RTI_DomainParticipant_i::RTI_DomainParticipant_i");
      }

      // Implementation skeleton destructor
      RTI_DomainParticipant_i::~RTI_DomainParticipant_i (void)
      {
        CIAO_TRACE ("RTI_DomainParticipant_i::~RTI_DomainParticipant_i");
      }

      ::DDS::Publisher_ptr
      RTI_DomainParticipant_i::create_publisher (const ::DDS::PublisherQos & qos,
                                                 ::DDS::PublisherListener_ptr a_listener,
                                                 ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_publisher");

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                     "Creating Publisher\n"));

        DDSPublisher * rti_pub =
          this->participant_->create_publisher (DDS_PUBLISHER_QOS_DEFAULT,
                                                0,
                                                mask);

        if (rti_pub == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                         "Error: Unable to create Participant\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG ((LM_INFO, CLINFO "RTI_DomainParticipant_i::create_publisher - "
                     "Successfully created a DDSPublisher\n"));

        ::DDS::Publisher_var retval = new RTI_Publisher_i (rti_pub);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_publisher (::DDS::Publisher_ptr p)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_publisher");

        RTI_Publisher_i *rti_pub = dynamic_cast < RTI_Publisher_i * > (p);

        if (rti_pub == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                         "Unable to cast provided object refence to servant pointer.\n"));
            return ::DDS::RETCODE_ERROR;
          }

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                     "Successfully casted provided object refence to RTI_Publisher_i\n"));

        DDS_ReturnCode_t retval = this->participant_->delete_publisher (rti_pub->get_publisher ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                         "Error: RTI delete_publisher returned non-ok error code %c\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG ((LM_INFO, CLINFO "RTI_DomainParticipant_i::delete_publisher - "
                          "Provided publisher successfully created\n"));

        return retval;
      }

      ::DDS::Subscriber_ptr
      RTI_DomainParticipant_i::create_subscriber (const ::DDS::SubscriberQos & qos,
                                                  ::DDS::SubscriberListener_ptr a_listener,
                                                  ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_subscriber");

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                     "Creating Subscriber\n"));

        DDSSubscriber * rti_sub =
          this->participant_->create_subscriber (DDS_SUBSCRIBER_QOS_DEFAULT,
                                                0,
                                                mask);

        if (!rti_sub)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                         "Error: Unable to create Subscriber\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG ((LM_INFO, CLINFO "RTI_DomainParticipant_i::create_subscriber - "
                     "Successfully created a DDSSubscriber\n"));

        ::DDS::Subscriber_var retval = new RTI_Subscriber_i (rti_sub);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_subscriber (::DDS::Subscriber_ptr s)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_subscriber");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::Subscriber_ptr
      RTI_DomainParticipant_i::get_builtin_subscriber (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_builtin_subscriber");
        throw CORBA::NO_IMPLEMENT ();

      }

      ::DDS::Topic_ptr
      RTI_DomainParticipant_i::create_topic (const char * topic_name,
                                             const char * type_name,
                                             const ::DDS::TopicQos & qos,
                                             ::DDS::TopicListener_ptr a_listener,
                                             ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_topic");

        if (topic_name == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: provided nil topic name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        if (type_name == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: provided nil type name\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER,
                                          0);
          }

        CIAO_DEBUG ((LM_DEBUG, CLINFO "DDS_DomainParticipant_i::create_topic - "
                     "Attempting to create topic with name %C and type %C\n",
                     topic_name, type_name));

        DDSTopic *rti_topic = this->participant_->create_topic (topic_name,
                                                                type_name,
                                                                DDS_TOPIC_QOS_DEFAULT,
                                                                0,
                                                                mask);

        if (rti_topic == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "DDS_DomainParticipant_i::create_topic - "
                         "Error: RTI DDS returned a nil topic\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        CIAO_DEBUG ((LM_INFO, CLINFO "DDS_DomainParticipant_i::create_topic - "
                     "Successfully created topic with name %C and type %C\n",
                     topic_name, type_name));

        ::DDS::Topic_var retval = new RTI_Topic_i (rti_topic);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipant_i::delete_topic (::DDS::Topic_ptr a_topic)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_topic");

        RTI_Topic_i *top = dynamic_cast< RTI_Topic_i *> (a_topic);

        if (top == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t retval = this->participant_->delete_topic (top->get_topic ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                         "Error: RTI delete_topic returned non-ok error code %c\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG ((LM_INFO, CLINFO "RTI_DomainParticipant_i::delete_topic - "
                          "Provided topic successfully deleted\n"));

        return retval;
      }

      ::DDS::Topic_ptr
      RTI_DomainParticipant_i::find_topic (const char * topic_name,
                                           const ::DDS::Duration_t & timeout)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::find_topic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::TopicDescription_ptr 
      RTI_DomainParticipant_i::lookup_topicdescription (const char * name)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::lookup_topicdescription");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ContentFilteredTopic_ptr 
      RTI_DomainParticipant_i::create_contentfilteredtopic (const char * name,
                                                            ::DDS::Topic_ptr related_topic,
                                                            const char * filter_expression,
                                                            const ::DDS::StringSeq & expression_parameters)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_contentfilteredtopic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::delete_contentfilteredtopic (::DDS::ContentFilteredTopic_ptr a_contentfilteredtopic)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_contentfilteredtopic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::MultiTopic_ptr 
      RTI_DomainParticipant_i::create_multitopic (const char * name,
                                                  const char * type_name,
                                                  const char * subscription_expression,
                                                  const ::DDS::StringSeq & expression_parameters)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::create_multitopic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::delete_multitopic (::DDS::MultiTopic_ptr a_multitopic)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_multitopic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::delete_contained_entities (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::delete_contained_entities");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::set_qos (const ::DDS::DomainParticipantQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_qos (::DDS::DomainParticipantQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::set_listener (::DDS::DomainParticipantListener_ptr a_listener,
                                             ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_listener");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::DomainParticipantListener_ptr 
      RTI_DomainParticipant_i::get_listener (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_listener");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::ignore_participant (::DDS::InstanceHandle_t handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::ignore_participant");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::ignore_topic (::DDS::InstanceHandle_t handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::ignore_topic");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::ignore_publication (::DDS::InstanceHandle_t handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::ignore_publication");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::ignore_subscription (::DDS::InstanceHandle_t handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::ignore_subscription");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::DomainId_t 
      RTI_DomainParticipant_i::get_domain_id (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_domain_id");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::assert_liveliness (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::assert_liveliness");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::set_default_publisher_qos (const ::DDS::PublisherQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_publisher_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_default_publisher_qos (::DDS::PublisherQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_publisher_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::set_default_subscriber_qos (const ::DDS::SubscriberQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_subscriber_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_default_subscriber_qos (::DDS::SubscriberQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_subscriber_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::set_default_topic_qos (const ::DDS::TopicQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::set_default_topic_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_default_topic_qos (::DDS::TopicQos & qos)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_discovered_participants (::DDS::InstanceHandleSeq & participant_handles)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_default_topic_qos");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_discovered_participant_data (::DDS::ParticipantBuiltinTopicData & participant_data,
                                                                ::DDS::InstanceHandle_t participant_handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_discovered_participant_data");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_discovered_topics (::DDS::InstanceHandleSeq & topic_handles)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_discovered_topics");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_discovered_topic_data (::DDS::TopicBuiltinTopicData & topic_data,
                                                          ::DDS::InstanceHandle_t topic_handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_discovered_topic_data");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::CORBA::Boolean 
      RTI_DomainParticipant_i::contains_entity (::DDS::InstanceHandle_t a_handle)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::contains_entity");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::get_current_time (::DDS::Time_t & current_time)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_current_time");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::ReturnCode_t 
      RTI_DomainParticipant_i::enable (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::enable");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::StatusCondition_ptr 
      RTI_DomainParticipant_i::get_statuscondition (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_statuscondition");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::StatusMask 
      RTI_DomainParticipant_i::get_status_changes (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_status_changes");
        throw CORBA::NO_IMPLEMENT ();
        
      }

      ::DDS::InstanceHandle_t 
      RTI_DomainParticipant_i::get_instance_handle (void)
      {
        CIAO_TRACE ("DDS_DomainParticipant_i::get_instance_handle");
        throw CORBA::NO_IMPLEMENT ();
        
      }
      
      DDSDomainParticipant * 
      RTI_DomainParticipant_i::get_participant (void)
      {
        return this->participant_;
      }
    }
  }
}
